// Задача:
// На вход подается очень большой файл, на каждой строчке которого
// содержится по слову. Необходимо  найти в нем ТОП 10 наиболее
// часто втсречающихся слов. При этом должно быть использовано
// не более 1 Кб памяти. Слова гаранитровано достаточно короткие,
// чтобы любые 10 их них поместились в память.
// Комментарий:
// Посеольку входными данными является конечный файл, а место
// на жестком диске и время выполнения по условию никак не
// ограничены, то для решения будет использован метод
// внешней сортировки слиянием. Исходный файл будет разбит
// на небольшие части, каждая из которых будет отсортировна,
// а затем будет выполнено слияние получившихся файлов.
// Затем в получившемся файле будут найденты требуемые слова.
// Для тестирования были созданы файлы с входными данными
// input_words_100k.txt и input_numbers_100k.txt,
// а так же файлы, содержащие требуемый результат
// input_words_100k_answer.txt и input_numbers_100k_answer.txt.


#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>


#include "functions.h"

#define BUF_SIZE 10
#define MAX_STR_LEN 20
#define TOP_N 10 // MUST BE NO MORE THAN BUF_SIZE
#define FILENAME_LEN 20


int main(int argc, char ** argv)
{

    FILE * F = NULL; // Фйал, откуда считываются данные.
    char ** buf; // Буфер, в котором хранятся данные
    size_t lower_cnt = 0;
    size_t upper_cnt = 0;
    char name1[FILENAME_LEN];
    char name2[FILENAME_LEN];
    size_t i = 0;
    size_t j = 0;
    size_t tmp = 0;
    size_t all_vars_size = 0;

// --------------------------------------------------
// Preparations

    if (argc < 2)
    {
        perror("Provide file name\n");
        return -1;
    }

    F = fopen(argv[1], "r");

    if (F == NULL)
    {
        perror("Cannot open file\n");
        return -2;
    }

    if (TOP_N > BUF_SIZE)
    {
        perror("Cannot find more elements than can fit in memory\n");
        return -3;
    }

    buf = (char**)malloc(sizeof(char*)*BUF_SIZE);
    for (i = 0; i < BUF_SIZE; i++)
    {
        buf[i] = (char*)malloc(sizeof(char)*MAX_STR_LEN);
        memset(buf[i], 0, sizeof(char)*MAX_STR_LEN);
    }
    memset(name1, 0, sizeof(char)*FILENAME_LEN);
    memset(name2, 0, sizeof(char)*FILENAME_LEN);

// --------------------------------------------------
// Action

    // Разбиваем исходный файл на сортированые фрагменты
    while( !feof(F) )
    {
        if (get_next(F, buf, BUF_SIZE, MAX_STR_LEN, &upper_cnt) == 0)
        {
            break;
        }
    }

    // Выполняем слияние и получаем отсортированный файл
    merge(MAX_STR_LEN, &upper_cnt);

    // Ищем ТОП-N слов и выводим их на печать
    find_top_n(buf, TOP_N, MAX_STR_LEN, &upper_cnt);



// --------------------------------------------------

    for (i = 0; i < BUF_SIZE; i++)
    {
        free(buf[i]);
    }
    free(buf);
    fclose(F);
    return 0;
}
