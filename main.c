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

#include "defines.h"
#include "functions.h"

int main(int argc, char ** argv)
{

    FILE * F = NULL; // Фйал, откуда считываются данные.
    char ** buf; // Буфер, в котором хранятся данные
    size_t upper_cnt = 0;
    size_t i = 0;
    size_t j = 0;
    size_t tmp = 0;
    size_t all_vars_size = 0;
    double tim = .0;
    struct timeval tv1, tv2;

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

// --------------------------------------------------
// Action

    gettimeofday(&tv1, NULL);


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

    gettimeofday(&tv2, NULL);
    tim = ((tv2.tv_sec + (double)tv2.tv_usec/1000000.0) - (tv1.tv_sec + (double)tv1.tv_usec/1000000.0));
    printf("time taken: %1.3f seconds\n", tim);
    all_vars_size = \
            // main
            sizeof(F) + sizeof(char**) + sizeof(char)*BUF_SIZE* MAX_STR_LEN \
            + sizeof(size_t)*5 + sizeof(tim) + sizeof(tv1)*2 \
            + sizeof(argc) + strlen(argv[0]) + strlen(argv[1]) \
            // remove_nl
            + sizeof(size_t) + sizeof('\n') \
            // get_next
            + sizeof(size_t)*3 + sizeof(FILE*) + sizeof(char)*FILENAME_LEN \
            // swap_strings
            + sizeof(char*) \
            // merge
            + sizeof(size_t)*3 + sizeof(FILE*)*3 + sizeof(char)*3*FILENAME_LEN + sizeof(char*)*2 \
            // find_top_n
            + sizeof(char)*FILENAME_LEN + sizeof(char*)*3 + sizeof(char)*MAX_STR_LEN*2 \
            + sizeof(size_t)*4 + sizeof(size_t*) + sizeof(size_t)*TOP_N + sizeof(FILE*) \
            // find_min
            + sizeof(size_t)*3 \
            // sort_buf
            + sizeof(size_t)*2 \
            // swap_buffer_elements
            + sizeof(char*);

    printf("Memory used: %u bytes\n", all_vars_size);
    printf("NB: string constants such as warning messages are not taken into account\n because they are not affecting the algotithm directly.\n");
// --------------------------------------------------

    for (i = 0; i < BUF_SIZE; i++)
    {
        free(buf[i]);
    }
    free(buf);
    fclose(F);
    return 0;
}
