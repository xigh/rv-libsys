# sys-utils

Specific operating system functions and defines :

```c
extern int rv_read_file(char *name, rv_buf_t *buf);
extern void rv_free_buf(rv_buf_t *buf);
```

```c
rv_bool_t
rv_int8_t
rv_uint8_t
rv_int16_t
rv_uint16_t
rv_int32_t
rv_uint32_t
rv_int64_t
rv_uint64_t
rv_float32_t
rv_float64_t
```

```c
struct rv_buf_t
{
    rv_uint64_t len;
    rv_uint8_t *data;
};
typedef struct rv_buf_t rv_buf_t;
```
