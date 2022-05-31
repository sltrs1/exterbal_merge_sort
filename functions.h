#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stddef.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

// Удаление символа \n
// Для облегчения печати
void remove_nl(char * str);

// Вычитать из файла следующий буфер и поместить его во временный файл
size_t get_next(FILE * F, char ** buf, size_t buf_size, size_t max_str_len, size_t * file_counter);

// Смена местами двух строк
void swap_strings(char ** str1_ptr, char ** str2_ptr);

// Выполнить слияние временых файлов
size_t merge(size_t max_str_len, size_t * file_counter);

#endif // FUNCTIONS_H
