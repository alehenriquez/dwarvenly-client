#define FILE_H

#ifndef TYPES_H
#include <locust/type.types.h>
#endif

#ifndef STRING_H
#include <locust/type.string.h>
#endif

#ifndef ERROR_H
#include <locust/type.error.h>
#endif

typedef struct {
    ptr handle;
    string name;
    byte mode;
} file;

#define file__type_id ((type_id)25)
extern TYPE_ID_DECL(file);
extern META_DECL(file);

// File mode arguments
enum {    
    // Mix in any of the following
    READ  = 0x01,
    WRITE = 0x02,
    EXEC  = 0x04,
};

// file errors
extern error file_err_not_file;
extern error file_err_unreadable;
extern error file_err_unwriteable;
extern error file_err_path_cycle;
extern error file_err_long_name;
extern error file_err_not_dir;
extern error file_err_bad_mode;

// file checks
error* file_exists(string filename);
error* file_check(string filename, byte mode);

// file manipulations
error* file_create();
error* file_open(file *f, string filename, byte mode);
error* file_close(file *f);

// reading functions
error* file_read(file *f, ptr str);

// writing functions
error* file_write(file *f, string str);

