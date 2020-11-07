#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

#define SUCCESS_CODE ((int)(0))
#define FAILURE_CODE ((int)(-1))
#define COMMAND_LINE_MAX_LENGTH 100

char* get_forest(int forest_size, FILE* file_pointer);
int call_process(char* command_line);
int get_num_of_burned(char* forest);
int write_to_file(FILE* file_ptr, char* line);
int create_output_file(char* forest, int num_of_gens);
#pragma once
