#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <stdio.h>

int check_read_permission(const char*);
int check_write_permission(const char*);
FILE* get_file_pointer(const char*);
int read_file(const char*);
int save_file(const char*, const char*);

#endif  // FILE_MANAGER_H
