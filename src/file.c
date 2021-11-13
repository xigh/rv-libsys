#include "rv-sys.h"

#ifdef _WIN32
#include <windows.h>
#include <stdio.h>

int rv_read_file(char *name, rv_buf_t *buf)
{
    HANDLE hFile;
    LARGE_INTEGER fileSize;
    BOOL bRet;
    
    hFile = CreateFileA(name, GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == NULL)
    {
        printf("%s could not get file size\n", name);
        CloseHandle(hFile);
        return -1;
    }

    bRet = GetFileSizeEx(hFile, &fileSize);
    if (!bRet)
    {
        printf("%s could not get file size\n", name);
        CloseHandle(hFile);
        return -1;
    }

    if (fileSize.HighPart != 0)
    {
        printf("%s file too big\n", name);
        CloseHandle(hFile);
        return -1;
    }

    buf->len = (rv_uint64_t) fileSize.LowPart;
    buf->data = HeapAlloc(GetProcessHeap(), 0, buf->len);
    if (buf->data == 0)
    {
        printf("%s could not allocate memory\n", name);
        CloseHandle(hFile);
        return -1;
    }

    DWORD bytesRead;
    bRet = ReadFile(hFile, buf->data, fileSize.LowPart, &bytesRead, NULL);
    CloseHandle(hFile);
    if (!bRet)
    {
        printf("%s could not read file\n", name);
        HeapFree(GetProcessHeap(), 0, buf->data);
        return -1;
    }

    rv_uint64_t r = (rv_uint64_t) bytesRead;
    if (r != buf->len)
    {
        printf("%s read incomplete %lld vs %lld\n", name, r, buf->len);
        HeapFree(GetProcessHeap(), 0, buf->data);
        return -1;
    }

    return 0;
}

void rv_free_buf(rv_buf_t *buf)
{
    HeapFree(GetProcessHeap(), 0, buf->data);
}
#else
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int rv_read_file(char *name, rv_buf_t *buf)
{
    struct stat sb;
    int err, fd;
    int64_t n;

    fd = open(name, O_RDONLY);
    if (fd < 0)
        return fd;

    err = fstat(fd, &sb);
    if (err < 0)
    {
        close(fd);
        return err;
    }

    if (sb.st_size > 128 * 1024 * 1024)
    {
        printf("%s file too big\n", name);
        close(fd);
        return -1;
    }

    buf->len = (rv_uint64_t) sb.st_size;
    buf->data = malloc(buf->len);
    if (buf->data == 0)
    {
        printf("%s could not allocate memory\n", name);
        close(fd);
        return -1;
    }

    n = read(fd, buf->data, buf->len);
    if (n < 0)
    {
        printf("%s read failed\n", name);
        close(fd);
        free(buf->data);
        return (int) n;
    }

    if ((rv_uint64_t) n != buf->len)
    {
        printf("%s read incomplete %lld vs %lld\n", name, (rv_uint64_t) n, buf->len);
        close(fd);
        free(buf->data);
        return (int) n;
    }

    close(fd);
    return 0;
}

void rv_free_buf(rv_buf_t *buf)
{
    free(buf->data);
}
#endif
