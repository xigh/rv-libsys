#ifndef __RV_SYS_H
#define __RV_SYS_H

#include <stdarg.h>

typedef char rv_bool_t;
typedef signed char rv_int8_t;
typedef unsigned char rv_uint8_t;
typedef signed short rv_int16_t;
typedef unsigned short rv_uint16_t;
typedef signed int rv_int32_t;
typedef unsigned int rv_uint32_t;
typedef signed long long rv_int64_t;
typedef unsigned long long rv_uint64_t;
typedef float rv_float32_t;
typedef double rv_float64_t;

struct rv_buf_t
{
    rv_uint64_t len;
    rv_uint8_t *data;
};
typedef struct rv_buf_t rv_buf_t;

extern int rv_read_file(char *name, rv_buf_t *buf);
extern void rv_free_buf(rv_buf_t *buf);

#endif // __RV_SYS_H
