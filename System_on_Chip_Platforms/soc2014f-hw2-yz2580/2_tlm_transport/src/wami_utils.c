#include "wami_utils.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

void *xmalloc(size_t size, const char *file, int line)
{
    void *x = malloc(size);
    if (x == NULL)
    {
        fprintf(stderr, "Error: memory allocation of size %lu at %s:%d.\n",
            size, file, line);
        exit(EXIT_FAILURE);
    }
    return x;
}

void concat_dir_and_filename(
    char dir_and_filename[MAX_DIR_AND_FILENAME_LEN],
    const char *directory,
    const char *filename)
{
    assert(dir_and_filename != NULL);
    assert(directory != NULL);
    assert(filename != NULL);

    /* C89 lacks snprintf */
    if (strlen(directory) + strlen(filename) + 2 > MAX_DIR_AND_FILENAME_LEN)
    {
        fprintf(stderr, "Error: input directory (%s) too long.\n",
            directory);
        exit(EXIT_FAILURE);
    }
    dir_and_filename[0] = '\0';
    strncpy(dir_and_filename, directory, MAX_DIR_AND_FILENAME_LEN-1);
    dir_and_filename[MAX_DIR_AND_FILENAME_LEN-1] = '\0';
    strncat(dir_and_filename, "/",
        MAX_DIR_AND_FILENAME_LEN - strlen(dir_and_filename) - 1);
    strncat(dir_and_filename, filename,
        MAX_DIR_AND_FILENAME_LEN - strlen(dir_and_filename) - 1);
}

void read_data_file(
    void *data,
    size_t size,
    const char *filename,
    const char *directory,
    size_t num_bytes)
{
    size_t nread = 0;
    FILE *fp = NULL;
    char dir_and_filename[MAX_DIR_AND_FILENAME_LEN];

    assert(data != NULL);
    assert(filename != NULL);
    assert(directory != NULL);

    concat_dir_and_filename(
        dir_and_filename,
        directory,
        filename);

    fp = fopen(dir_and_filename, "rb");
    if (fp == NULL)
    {
        fprintf(stderr, "Error: Unable to open input file %s for reading.\n",
            dir_and_filename);
        exit(EXIT_FAILURE);
    }

    nread = fread(data, size, num_bytes/size, fp);
    if (nread != num_bytes/size)
    {
        fprintf(stderr, "Error: read failure on %s. "
            "Expected %lu bytes, but only read %lu.\n",
            dir_and_filename, num_bytes, nread);
        fclose(fp);
        exit(EXIT_FAILURE);
    }        

    fclose(fp);
}

void read_image_file(
    void *data,
    size_t size,
    const char *filename,
    const char *directory,
    size_t num_bytes)
{
    int success = 1;
    size_t nread = 0, num_bytes_reported_by_header;
    FILE *fp = NULL;
    char dir_and_filename[MAX_DIR_AND_FILENAME_LEN];
    u16 width, height, channels, depth;

    assert(data != NULL);
    assert(filename != NULL);
    assert(directory != NULL);

    concat_dir_and_filename(
        dir_and_filename,
        directory,
        filename);

    fp = fopen(dir_and_filename, "rb");
    if (fp == NULL)
    {
        fprintf(stderr, "Error: Unable to open input file %s for reading.\n",
            dir_and_filename);
        exit(EXIT_FAILURE);
    }

    success &= (fread(&width, sizeof(u16), 1, fp) == 1);
    success &= (fread(&height, sizeof(u16), 1, fp) == 1);
    success &= (fread(&channels, sizeof(u16), 1, fp) == 1);
    success &= (fread(&depth, sizeof(u16), 1, fp) == 1);
    if (! success)
    {
        fprintf(stderr, "Error: header read failure on %s. ", dir_and_filename);
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    num_bytes_reported_by_header = (size_t) width * height * channels * depth;
    if (num_bytes_reported_by_header != num_bytes)
    {
        fprintf(stderr, "Error: header dimensions in %s inconsistent with requested "
            "number of bytes to read (width=%u,height=%u,channels=%u,depth=%u)\n",
            dir_and_filename, width, height, channels, depth);
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    nread = fread(data, size, num_bytes/size, fp);
    if (nread != num_bytes/size)
    {
        fprintf(stderr, "Error: read failure on %s. "
            "Expected %lu bytes, but only read %lu.\n",
            dir_and_filename, num_bytes, nread);
        fclose(fp);
        exit(EXIT_FAILURE);
    }        

    fclose(fp);
}

void write_image_file(
    void *data,
    size_t size,
    const char *filename,
    const char *directory,
    u16 width,
    u16 height,
    u16 channels)
{
    int success = 1;
    size_t nwritten = 0, num_bytes = 0;
    FILE *fp = NULL;
    char dir_and_filename[MAX_DIR_AND_FILENAME_LEN];
    const u16 depth = 2;

    assert(data != NULL);
    assert(filename != NULL);
    assert(directory != NULL);

    concat_dir_and_filename(
        dir_and_filename,
        directory,
        filename);

    fp = fopen(dir_and_filename, "wb");
    if (fp == NULL)
    {
        fprintf(stderr, "Error: Unable to open input file %s for reading.\n",
            dir_and_filename);
        exit(EXIT_FAILURE);
    }

    success &= (fwrite(&width, sizeof(u16), 1, fp) == 1);
    success &= (fwrite(&height, sizeof(u16), 1, fp) == 1);
    success &= (fwrite(&channels, sizeof(u16), 1, fp) == 1);
    success &= (fwrite(&depth, sizeof(u16), 1, fp) == 1);
    if (! success)
    {
        fprintf(stderr, "Error: header write failure on %s. ", dir_and_filename);
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    num_bytes = (size_t) width * height * channels * depth;

    nwritten = fwrite(data, size, num_bytes/size, fp);
    if (nwritten != num_bytes/size)
    {
        fprintf(stderr, "Error: write failure on %s. "
            "Expected to write %lu bytes, but only wrote %lu.\n",
            dir_and_filename, num_bytes, nwritten);
        fclose(fp);
        exit(EXIT_FAILURE);
    }        

    fclose(fp);
}
