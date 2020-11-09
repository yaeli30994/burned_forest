#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <stdbool.h>

#define SUCCESS_CODE ((int)(0))
#define FAILURE_CODE ((int)(-1))
#define COMMAND_LINE_MAX_LENGTH 100
#define EXRTA_CHARECTERS 6;

char* get_forest(int forest_size, FILE* file_pointer);
int call_process(char* command_line);
int get_num_of_burned(char* forest);
int write_to_file(FILE* file_ptr, char* line);
int create_output_file(char* forest, int num_of_gens, int forest_size);
char* forest_next_step(int forest_size, char* forest_string);
void Get_neighbours(int i, char* forest_string, int forest_size, char* neighbours_total, char* neighbours_plus_shape, int* num_total, int* num_plus);
bool top_row(int i, int forest_size);
bool bottom_row(int i, int forest_size);
bool left_column(int i, int forest_size);
bool right_column(int i, int forest_size);
void Check_neighbours_and_update_cell(int i, char* forest_string, char* neighbours_total, char* neighbours_plus_shape, int* num_total, int* num_plus, char* next_step_forest);
#pragma once
