cmake_minimum_required(VERSION 3.5)

project(external_merge_sort LANGUAGES C)

add_executable(external_merge_sort main.c defines.h functions.h functions.c)
