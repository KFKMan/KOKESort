#ifndef SECUREFUNCTIOONS_H
#define SECUREFUNCTIOONS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

//https://stackoverflow.com/a/2524673
#ifndef SIZE_T_IDENTIFIER
    #if defined(_MSC_VER)
        #define SIZE_T_IDENTIFIER "%Iu"
    #elif defined(__GNUC__) || defined(__clang__)
        #define SIZE_T_IDENTIFIER "%zu"
    #else
        #define SIZE_T_IDENTIFIER "%zu"
    #endif
#endif

#ifndef _MSC_VER

// strcpy_s için makro
#ifndef strcpy_s
#define strcpy_s(dest, dest_size, src) \
    (((dest) && (src) && (dest_size) > strlen(src)) ? \
     (strcpy(dest, src), 0) : \
     (errno = EINVAL, -1))
#endif

// strcat_s için makro
#ifndef strcat_s
#define strcat_s(dest, dest_size, src) \
    (((dest) && (src) && (dest_size) > (strlen(dest) + strlen(src))) ? \
     (strcat(dest, src), 0) : \
     (errno = EINVAL, -1))
#endif

// sprintf_s için makro
#ifndef sprintf_s
#define sprintf_s(dest, dest_size, ...) \
    (((dest) && (dest_size) > 0) ? \
     (snprintf(dest, dest_size, __VA_ARGS__)) : \
     (errno = EINVAL, -1))
#endif

// fopen_s için makro
#ifndef fopen_s
#define fopen_s(fp, filename, mode) \
    (((fp) && (filename) && (mode)) ? \
     (((*(fp) = fopen(filename, mode)) != NULL) ? 0 : errno) : \
     (errno = EINVAL, -1))
#endif

// getenv_s için makro
#ifndef getenv_s
#define getenv_s(size, buffer, buffer_size, varname) \
    ((buffer && varname && buffer_size > 0) ? \
     ((strncpy(buffer, getenv(varname), buffer_size - 1)), \
      (buffer[buffer_size - 1] = '\0'), 0) : \
     (errno = EINVAL, -1))
#endif

#endif
#endif