#include "Father.h"

int main(int argc, char* argv[])
{
	if (argc < 2) {
		printf("Not enough arguments.\n");
		exit(1);
	}
	FILE* file_pointer = NULL;
	if (fopen_s(&file_pointer, argv[1], "r") != 0) 
	{
		printf("couldn't read file");
		exit(1);
	}
	char* line = malloc(sizeof(char) * 4);
	int num_of_gens = 0, forest_size = 0;
	// get the first 2 lines (size of forest and generations) and convert them to int
	fgets(line, sizeof(line), file_pointer);
	forest_size = (int)line[0] - '0';
	fgets(line, sizeof(line), file_pointer);
	num_of_gens = (int)line[0] - '0';
	free(line);
	int num_of_trees = forest_size * forest_size;
	char* forest = get_forest(num_of_trees, file_pointer);
	fclose(file_pointer);
	create_output_file(forest, num_of_gens, forest_size);
	return 0;
}

char* get_forest(int num_of_trees, FILE* file_pointer) {
	// the function gets the number of trees in the forest and the file pointer
	// and returns the forest as a string
	char* forest = malloc((num_of_trees + 1) * sizeof(char));
	char current_char;
	int i = 0;
	while ((current_char = (char)fgetc(file_pointer)) != EOF) {
		if (current_char != ',' && current_char != '\n') {
			forest[i] = current_char;
			i++;
		}
	}
	forest[i] = '\0';
	return forest;
}

int call_process(char* command_line) {
	// calls a process using the given commad line
	STARTUPINFO	startinfo = { sizeof(STARTUPINFO), NULL, 0 };
	PROCESS_INFORMATION pi;
	PROCESS_INFORMATION* ProcessInfoPtr = &pi;
	DWORD exit_code;
	WCHAR w_command_line[COMMAND_LINE_MAX_LENGTH];
	size_t w_command_line_size;
	mbstowcs_s(&w_command_line_size, w_command_line, COMMAND_LINE_MAX_LENGTH, command_line, COMMAND_LINE_MAX_LENGTH);
	if (!CreateProcess(NULL, w_command_line, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &startinfo, ProcessInfoPtr))
	{
		printf("Failed to create process (%d).\n", GetLastError());
		return FAILURE_CODE;
	}
	// Wait until son.exe exits.
	WaitForSingleObject(pi.hProcess, INFINITE);
	GetExitCodeProcess(pi.hProcess, &exit_code);
	return (int)exit_code;
}

int get_num_of_burned(char* forest) {
	// this function calculates the number of burned trees in the furest using son.exe
	//it creates the needed command line and calls call_process to run it
	int burned_trees_num = 0;
	char command_line[COMMAND_LINE_MAX_LENGTH];
	snprintf(command_line, COMMAND_LINE_MAX_LENGTH, "Son.exe %s", forest);

	burned_trees_num = call_process(command_line);
	if (burned_trees_num == FAILURE_CODE) {
		return FAILURE_CODE;
	}
	return burned_trees_num;

}

int write_to_file(FILE* file_ptr, char* line) {
	// this function writes the given line to the given file
	if (EOF == fputs(line, file_ptr))
	{
		printf("Failed to write to file.\n");
		if (fclose(file_ptr) != 0)
		{
			printf("Failed to close file.\n");
			return FAILURE_CODE;
		}
		return FAILURE_CODE;
	}
	return SUCCESS_CODE;
}

int create_output_file(char* forest, int num_of_gens, int forest_size) {
	// this function creates output.txt and calles all necessary functions in order to do so
	FILE* file_pointer = NULL;
	if (fopen_s(&file_pointer, "output.txt", "w") != 0) {
		printf("couldn't open file");
		exit(1);
	}
	for (int i = 0; i < num_of_gens; i++)
	{
		int num_of_burned = get_num_of_burned(forest);
		int num_length = snprintf(NULL, 0, "%d", num_of_burned);
		int line_size = num_length + strlen(forest) + EXRTA_CHARECTERS;
		char* line = malloc((line_size) * sizeof(char));
		snprintf(line, line_size, "%s - %d\n", forest, num_of_burned);
		write_to_file(file_pointer, line);
		forest = forest_next_step(forest_size, forest);
	}
	if (fclose(file_pointer) != 0)
	{
		printf("Failed to close file.\n");
		return FAILURE_CODE;
	}
	return SUCCESS_CODE;
}

char* forest_next_step(int forest_size, char *forest_string)//creates another "board"/forest on which we will update the progression after one iteration
{
	char *next_step_forest = (char*)malloc(((forest_size*forest_size)+1) * sizeof(char));
	char neighbours_plus[4];//neighbours above, below, left, right
	char neighbours_total[8];//neighbours above, below, left, right and diagonally
	int i,num_neighbours_total_in_array, num_neighbours_plus_in_array; //num_neighbours= how many neighbours exist for a cell. for example, the top corner only has 2 "plus" neighbours (out of the array whose size is 4, and only 3 total neighbours out of the array whose size is 8)
	if (forest_size == 1)
	{
		return forest_string;
	}
	for (i = 0; i<(forest_size*forest_size); i++)
	{
		Get_neighbours(i, forest_string, forest_size, &neighbours_total, &neighbours_plus, &num_neighbours_total_in_array, &num_neighbours_plus_in_array);
		Check_neighbours_and_update_cell(i, forest_string, &neighbours_total, &neighbours_plus, &num_neighbours_total_in_array, &num_neighbours_plus_in_array, next_step_forest);
	}
	next_step_forest[forest_size * forest_size]= '\0';
	strcpy(forest_string, next_step_forest);
	free(next_step_forest);
	return forest_string;
}


void Get_neighbours(int i, char *forest_string, int forest_size, char *neighbours_total, char *neighbours_plus_shape, int *num_total, int *num_plus)//checks how many neighbours exist in "plus" shape, and in total (including diagonal neighbours)
{
	if (top_row(i, forest_size))
	{
		if (left_column(i, forest_size)) //top left corner
		{
			neighbours_plus_shape[0] = forest_string[i + 1];
			neighbours_plus_shape[1] = forest_string[i + forest_size];
			*num_plus = 2;//how many neighbours above, below, to the right or left there are for each cell (out of 4 maximum)
			neighbours_total[0] = forest_string[i + 1];
			neighbours_total[1] = forest_string[i + forest_size];
			neighbours_total[2] = forest_string[i + forest_size + 1];
			*num_total = 3;//how many neighbours above, below,diagonally or to the right or left there are for each cell (out of 8 maximum)
		}
		else if (right_column(i, forest_size)) //top right corner
		{
			neighbours_plus_shape[0] = forest_string[i + forest_size];
			neighbours_plus_shape[1] = forest_string[i - 1];
			*num_plus = 2;
			neighbours_total[0] = forest_string[i + forest_size];
			neighbours_total[1] = forest_string[i - 1];
			neighbours_total[2] = forest_string[i +forest_size - 1];
			*num_total = 3;
		}
		else //top row but not corner
		{
			neighbours_plus_shape[0] = forest_string[i + 1];
			neighbours_plus_shape[1] = forest_string[i + forest_size];
			neighbours_plus_shape[2] = forest_string[i - 1];
			*num_plus = 3;
			neighbours_total[0] = forest_string[i + 1];
			neighbours_total[1] = forest_string[i + forest_size];
			neighbours_total[2] = forest_string[i - 1];
			neighbours_total[3] = forest_string[i +forest_size - 1];
			neighbours_total[4] = forest_string[i + forest_size + 1];
			*num_total = 5;
		}
	}
	else if (bottom_row(i, forest_size))
	{
		if (left_column(i, forest_size)) //bottom left corner
		{
			neighbours_plus_shape[0] = forest_string[i + 1];
			neighbours_plus_shape[1] = forest_string[i - forest_size];
			*num_plus = 2;
			neighbours_total[0] = forest_string[i + 1];
			neighbours_total[1] = forest_string[i - forest_size];
			neighbours_total[2] = forest_string[i - forest_size + 1];
			*num_total = 3;
		}
		else if (right_column(i, forest_size)) //bottom right corner
		{
			neighbours_plus_shape[0] = forest_string[i - forest_size];
			neighbours_plus_shape[1] = forest_string[i - 1];
			*num_plus = 2;
			neighbours_total[0] = forest_string[i - forest_size];
			neighbours_total[1] = forest_string[i - 1];
			neighbours_total[2] = forest_string[i - forest_size - 1];
			*num_total = 3;
		}
		else //bottom row but not corner
		{
			neighbours_plus_shape[0] = forest_string[i + 1];
			neighbours_plus_shape[1] = forest_string[i - forest_size];
			neighbours_plus_shape[2] = forest_string[i - 1];
			*num_plus = 3;
			neighbours_total[0] = forest_string[i + 1];
			neighbours_total[1] = forest_string[i - forest_size];
			neighbours_total[2] = forest_string[i - 1];
			neighbours_total[3] = forest_string[i - forest_size - 1];
			neighbours_total[4] = forest_string[i - forest_size + 1];
			*num_total = 5;
		}
	}
	else if (left_column(i, forest_size)) //but not the corners
	{
		neighbours_plus_shape[0] = forest_string[i + 1];
		neighbours_plus_shape[1] = forest_string[i - forest_size];
		neighbours_plus_shape[2] = forest_string[i + forest_size];
		*num_plus = 3;
		neighbours_total[0] = forest_string[i + 1];
		neighbours_total[1] = forest_string[i - forest_size];
		neighbours_total[2] = forest_string[i + forest_size];
		neighbours_total[3] = forest_string[i - forest_size+1];
		neighbours_total[4] = forest_string[i + forest_size+1];
		*num_total = 5;
	}
	else if (right_column(i, forest_size)) //but not the corners
	{
		neighbours_plus_shape[0] = forest_string[i - 1];
		neighbours_plus_shape[1] = forest_string[i - forest_size];
		neighbours_plus_shape[2] = forest_string[i + forest_size];
		*num_plus = 3;
		neighbours_total[0] = forest_string[i - 1];
		neighbours_total[1] = forest_string[i - forest_size];
		neighbours_total[2] = forest_string[i + forest_size];
		neighbours_total[3] = forest_string[i - forest_size - 1];
		neighbours_total[4] = forest_string[i + forest_size - 1];
		*num_total = 5;
	}
	else// not on the borders of the forest
	{
		neighbours_plus_shape[0] = forest_string[i + 1];
		neighbours_plus_shape[1] = forest_string[i - forest_size];
		neighbours_plus_shape[2] = forest_string[i - 1];
		neighbours_plus_shape[3] = forest_string[i + forest_size];
		*num_plus = 4;
		neighbours_total[0] = forest_string[i + 1];
		neighbours_total[1] = forest_string[i - forest_size];
		neighbours_total[2] = forest_string[i + forest_size];
		neighbours_total[3] = forest_string[i - 1];
		neighbours_total[4] = forest_string[i - forest_size - 1];
		neighbours_total[5] = forest_string[i - forest_size + 1];
		neighbours_total[6] = forest_string[i + forest_size - 1];
		neighbours_total[7] = forest_string[i + forest_size + 1];
		*num_total = 8;
	}
}

bool top_row(int i, int forest_size)
{
	return (i / forest_size == 0);
}

bool bottom_row(int i, int forest_size)
{
	return (i / forest_size == forest_size-1);
}

bool left_column(int i, int forest_size)
{
	return (i % forest_size == 0);
}
bool right_column(int i, int forest_size)
{
	return (i % forest_size == forest_size - 1);
}

void Check_neighbours_and_update_cell(int i, char* forest_string, char* neighbours_total, char* neighbours_plus_shape, int* num_total, int* num_plus,char *next_step_forest)
{
	int j, tree_count = 0, no_fire_around_tree = 1;
	if (forest_string[i] == 'F')
	{
		next_step_forest[i] = 'G';
	}
	else if (forest_string[i] == 'T')
	{
		for (j = 0; j < *num_plus; j++)
		{
			if (neighbours_plus_shape[j] == 'F')
			{
				no_fire_around_tree = 0;
			}
		}
		if (no_fire_around_tree == 1)// there is no fire around the tree
			next_step_forest[i] = 'T';
		else//there is fire next to the tree
			next_step_forest[i] = 'F';
	}
	else// forest_string[i] = 'G'
	{
		for (j = 0; j < *num_total; j++)
		{
			if (neighbours_total[j] == 'T')
			{
				tree_count++;
			}
		}
		if (tree_count > 1)
			next_step_forest[i] = 'T';
		else
			next_step_forest[i] = 'G';
	}
}
