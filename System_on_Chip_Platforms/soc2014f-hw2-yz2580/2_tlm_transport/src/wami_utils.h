#ifndef _WAMI_UTILS_H_
#define _WAMI_UTILS_H_

#include <stdlib.h>
#include "wami_params.h"

#define MAX_DIR_AND_FILENAME_LEN (1024)

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

/* appends filename to directory */
void concat_dir_and_filename(
    char dir_and_filename[MAX_DIR_AND_FILENAME_LEN],
    const char *directory,
    const char *filename);

/* reads input data files */
void read_data_file(
    void *data,
    size_t size,
    const char *filename,
    const char *directory,
    size_t num_bytes);

/* reads RGB image files with a trivial header */
void read_image_file(
    void *data,
    size_t size,
    const char *filename,
    const char *directory,
    size_t num_bytes);

/* writes image files with a trivial header */
void write_image_file(
    void *data,
    size_t size,
    const char *filename,
    const char *directory,
    u16 width,
    u16 height,
    u16 channels);

/* error checked memory allocation */
#define XMALLOC(size) xmalloc(size, __FILE__, __LINE__)
void *xmalloc(size_t size, const char *file, int line);

#define FREE_AND_NULL(x) \
    do { \
        if (x) { free(x); } \
        x = NULL; \
    } while (0); 
        
#endif /* _WAMI_UTILS_H_ */
