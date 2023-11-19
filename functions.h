#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <sys/stat.h>
#include <stddef.h>

int fileExists(const char *fileName);
void print_usage();
int is_str_part_digit(char *arr[], int j, int n, size_t end);

#endif