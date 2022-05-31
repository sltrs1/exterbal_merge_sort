#include "functions.h"

//****************************************************************************
//****************************************************************************

void remove_nl(char * str)
{
    if (str == NULL)
    {
        perror("remove_eol: Invalid parameters\n");
        return;
    }

    size_t i = 0;

    for (i = 0; i < strlen(str); i++)
    {
        if (str[i] == '\n')
        {
            str[i] = 0;
            break;
        }
    }

}

//****************************************************************************
//****************************************************************************

size_t get_next(FILE * F, char ** buf, size_t buf_size, size_t max_str_len, size_t * file_counter)
{

    if (F == NULL || buf == NULL || buf_size == 0 || max_str_len == 0 || file_counter == NULL)
    {
        perror("get_next: Invalid parameters\n");
        return 0;
    }

    size_t i = 0;
    size_t j = 0;
    size_t num_read = 0;
    FILE * G = NULL;
    char filename[20];

    for (i = 0; i < buf_size; i++)
    {
        if (!fgets(buf[i], max_str_len, F))
        {
            break;
        }
        else
        {
            remove_nl(buf[i]);
            num_read++;
        }
    }

    for (i = 0; i < num_read; i++)
    {
        for (j = num_read - 1; j > i; j--)
        {
            if ( strcmp(buf[j-1], buf[j]) > 0 )
            {
                swap_strings(&buf[j-1], &buf[j]);
            }
        }
    }

    sprintf(filename, "%u.txt", *file_counter);

    G = fopen(filename, "w");

    if (G == NULL)
    {
        perror("Cannot open file\n");
        return 0;
    }

    for (i = 0; i < num_read; i++)
    {
        fprintf(G, "%s\n", buf[i]);
    }
    fclose(G);

    (*file_counter)++;

    return num_read;
}

//****************************************************************************
//****************************************************************************

void swap_strings(char ** str1_ptr, char ** str2_ptr)
{
    char * tmp = *str1_ptr;
    *str1_ptr = *str2_ptr;
    *str2_ptr = tmp;
}

//****************************************************************************
//****************************************************************************

size_t merge(size_t max_str_len, size_t * file_counter)
{
    if (max_str_len == 0 || *file_counter == 0)
    {
        perror("merge: Invalid parameters\n");
        return 0;
    }

    size_t num_done = 0;
    size_t to_do = 0;
    FILE * F1 = NULL;
    FILE * F2 = NULL;
    FILE * G = NULL;
    char file_to_merge1[20];
    char file_to_merge2[20];
    char next_file[20];
    char err_line[30];

    char * read_result1 = NULL;
    char * read_result2 = NULL;

    // Для хранения строк
    char * buf1 = (char*)malloc(sizeof(char)*max_str_len);
    char * buf2 = (char*)malloc(sizeof(char)*max_str_len);
    memset(buf1, 0, (sizeof(char)*max_str_len));
    memset(buf2, 0, (sizeof(char)*max_str_len));
    memset(file_to_merge1, 0, sizeof(char)*20);
    memset(file_to_merge2, 0, sizeof(char)*20);
    memset(err_line, 0, sizeof(char)*20);

    // Еси есть только один файл, то слияние выполнять не над чем
    if (*file_counter == 1)
    {
        free(buf1);
        free(buf2);
        return 0;
    }

    to_do = *file_counter - num_done;
    while( to_do > 1 )
    {
        sprintf(file_to_merge1, "%u.txt", num_done);
        sprintf(file_to_merge2, "%u.txt", num_done + 1);
        sprintf(next_file, "%u.txt", *file_counter);
        F1 = fopen(file_to_merge1, "r");
        sprintf(err_line, "errno = %i, ferror = %i\n", errno, ferror(F1));
        if (F1 == NULL)
        {
            perror("F1 is NULL\n");
        }
        F2 = fopen(file_to_merge2, "r");
        G = fopen(next_file, "w");

        read_result1 = fgets(buf1, max_str_len, F1);
        remove_nl(buf1);
        read_result2 = fgets(buf2, max_str_len, F2);
        remove_nl(buf2);

        // Сравниваем строки и записываем в файл наименьшую,
        // пока не кончится один из файлов
        while ( read_result1 != NULL && read_result2 != NULL )
        {
            if( strcmp(buf1, buf2) < 0 )
            {
                fprintf(G, "%s\n", buf1);
                read_result1 = fgets(buf1, max_str_len, F1);
                remove_nl(buf1);

            }
            else
            {
                fprintf(G, "%s\n", buf2);
                read_result2 = fgets(buf2, max_str_len, F2);
                remove_nl(buf2);
            }
        }

        //Проверяем, какой файл кончился и записываем остатки другого
        if (read_result1 == NULL && read_result2 != NULL)
        {
            while(read_result2 != NULL)
            {
                fprintf(G, "%s\n", buf2);
                read_result2 = fgets(buf2, max_str_len, F2);
                remove_nl(buf2);
            }
        }

        if (read_result1 != NULL && read_result2 == NULL)
        {
            while(read_result1 != NULL)
            {
                fprintf(G, "%s\n", buf1);
                read_result1 = fgets(buf1, max_str_len, F1);
                remove_nl(buf1);
            }
        }



        num_done += 2;
        *file_counter += 1;
        to_do = *file_counter - num_done;
        fclose(F1);
        fclose(F2);
        fclose(G);
        remove(file_to_merge1);
        remove(file_to_merge2);
    }

    free(buf1);
    free(buf2);
    return num_done;
}

//****************************************************************************
//****************************************************************************

size_t find_top_n(char ** buf, size_t top_n, size_t max_str_len, size_t * file_counter)
{
    if (buf == NULL || top_n == 0 || max_str_len == 0 || *file_counter == 0)
    {
        perror("find_top_n: Invalid parameters\n");
        return 0;
    }

    char filename[20];
    char * curr_str = (char*)malloc(sizeof(char)*max_str_len);
    char * next_str = (char*)malloc(sizeof(char)*max_str_len);
    size_t curr_cnt = 0;
    size_t next_cnt = 0;
    size_t idx = 0;
    size_t i = 0;
    size_t * cnt = (size_t*)malloc(sizeof(size_t)*max_str_len);
    FILE * F = NULL;
    char * read_result = NULL;

    sprintf(filename, "%u.txt", ((*file_counter)-1));
    F = fopen(filename, "r");
    memset(cnt, 0, sizeof(size_t)*top_n);
    memset(curr_str, 0, sizeof(char)*max_str_len);
    memset(next_str, 0, sizeof(char)*max_str_len);





    read_result = fgets(next_str, max_str_len, F);
    remove_nl(next_str);
    memcpy(curr_str, next_str, sizeof(char)*max_str_len);
    while(!feof(F))
    {
        if(read_result == NULL)
        {
            break;
        }

        if ( strcmp(curr_str, next_str) == 0)
        {
            curr_cnt++;
        }
        else
        {

            idx = find_min(cnt, top_n);
            if(cnt[idx] < curr_cnt)
            {
                cnt[idx] = curr_cnt;
                memcpy(buf[idx], curr_str, sizeof(char)*max_str_len);
            }
            memcpy(curr_str, next_str, sizeof(char)*max_str_len);
            curr_cnt = 1;
        }

        read_result = fgets(next_str, max_str_len, F);
        remove_nl(next_str);
        next_cnt = 1;
    }

    for (i = 0; i < top_n; i++)
    {
        printf("%2i - %s - %2i\n", i, buf[i], cnt[i]);
    }

    free(cnt);
    free(curr_str);
    fclose(F);
    return 1;
}

//****************************************************************************
//****************************************************************************

size_t find_min(size_t * cnt, size_t size)
{
    if (cnt == NULL || size == 0)
    {
        perror("find_min: Invalid parameters\n");
        return size + 1;
    }

    size_t val = cnt[0];
    size_t idx = 0;
    size_t i = 0;

    for(i = 1; i < size; i++)
    {
        if (cnt[i] < val)
        {
            val = cnt[i];
            idx = i;
        }
    }
    return idx;
}

//****************************************************************************
//****************************************************************************
