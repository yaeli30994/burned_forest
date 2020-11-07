#include "Father.h"

int main(int argc, char* argv[])
{
	if (argc < 2) {
		printf("Not enough arguments.\n");
		exit(1);
	}
	FILE* file_pointer = NULL;
	if (fopen_s(&file_pointer, argv[1], "r") != 0) {
		printf("couldn't read file");
		exit(1);
	}
	char *line = malloc(sizeof(char)*2);
	int num_of_gens = 0, forest_size = 0;
	fgets(line, sizeof(line), file_pointer);
	forest_size = (int)line[0] - '0';
	fgets(line, sizeof(line), file_pointer);
	num_of_gens = (int)line[0] - '0';
	int num_of_trees = forest_size*forest_size;
	printf("%d %d %d", forest_size, num_of_gens, num_of_trees); //for testing
	char* str = get_forest(num_of_trees, file_pointer);
	fclose(file_pointer);
	printf("%s", str);// for testing
	create_output_file(str, num_of_gens);
	return 0;
}

char *get_forest(int forest_size,  FILE* file_pointer ) {
	char *forest = malloc((forest_size+1) * sizeof(char));
	char current_char;
	int i = 0;
	while ((current_char = (char)fgetc(file_pointer)) != EOF){
		if (current_char != ',' && current_char != '\n') {
			forest[i] = current_char;
			i++;
		}

	} 
	forest[i] = '\0';
	return forest;
}

int call_process(char* command_line) {
	STARTUPINFO	startinfo = { sizeof(STARTUPINFO), NULL, 0 };
	PROCESS_INFORMATION pi;
	PROCESS_INFORMATION* ProcessInfoPtr = &pi;
	DWORD exit_code;
	WCHAR w_command_line[COMMAND_LINE_MAX_LENGTH];
	size_t w_command_line_size;
	mbstowcs_s(&w_command_line_size, w_command_line, COMMAND_LINE_MAX_LENGTH, command_line, COMMAND_LINE_MAX_LENGTH);
	if (!CreateProcess(NULL, command_line, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &startinfo, ProcessInfoPtr))
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

int create_output_file(char *forest, int num_of_gens) {
	FILE* file_pointer = NULL;
	if (fopen_s(&file_pointer, "output.txt", "w") != 0) {
		printf("couldn't open file");
		exit(1);
	}
	for (int i = 0; i < num_of_gens; i++)
	{
		int num_of_burned = get_num_of_burned(forest);
		int num_length = snprintf(NULL, 0, "%d", num_of_burned);
		int line_size = num_length + strlen(forest) + 15;
		char *line = malloc((line_size) * sizeof(char));
		snprintf(line, line_size, "%s - %d\n",forest, num_of_burned);
		write_to_file(file_pointer, line);
		//move to the next generation of the forest
	}
	if (fclose(file_pointer) != 0)
	{
		printf("Failed to close file.\n");
		return FAILURE_CODE;
	}
	return SUCCESS_CODE;
}