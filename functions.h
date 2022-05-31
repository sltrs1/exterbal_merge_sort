#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "defines.h"
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

// Поиск и печать ТОП-N слов в файле
size_t find_top_n(char ** buf, size_t top_n, size_t max_str_len, size_t * file_counter);

// Ищет счетчик с наименьшим значением
size_t find_min(size_t * cnt, size_t size);

// Сортировка буфера
void sort_buf(char ** buf, size_t * cnt, size_t buf_size);

// Смена местами элементов буфера и их счетчиков
void swap_buffer_elements(char ** str1_ptr, char ** str2_ptr, size_t * x, size_t * y);

#endif // FUNCTIONS_H
