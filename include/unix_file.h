#ifndef __UNIX_FILE_H__
#define __UNIX_FILE_H__

/* open file*/
#include "common.h"

int open_file(const char *, int);

/* lseek file*/
off_t lseek_file(int, off_t, int);

/* read file*/
ssize_t read_file(int, void *, size_t);

/* write file*/
ssize_t write_file(int, const void *, size_t);

/* 关闭文件*/
void close_file(int fd);

/* 写入文件内容*/
void write_content(int, const char *);

/* 读取文件内容*/
void read_content(int fd);

/* 删除文件*/
void delete_file(const char *);
#endif