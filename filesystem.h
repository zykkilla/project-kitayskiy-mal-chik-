#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdio.h>

typedef struct {
    char* name;
    char* content;
} FS_File;
int fs_init(const char* filename);
int fs_create(const char* filename, const char* content);
char* fs_read(const char* filename);
int fs_delete(const char* filename);
int fs_modify(const char* filename, const char* new_content);
void fs_close();

#endif
