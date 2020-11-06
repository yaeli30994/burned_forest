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
	int num_of_gens = 0, forest_size = 0;
	forest_size = (int)fgets(forest_size, 1, file_pointer) + 60;
	num_of_gens = (int)fgets(num_of_gens, 1, file_pointer) + 60;
	int string_size = forest_size^2;
	

	return 0;
}

char *get_forest(int forest_size,  FILE* file_pointer ) {
	char *forest = malloc(forest_size * sizeof(char));
	char current_char;
	int i = 0;
	do
	{
		current_char = (char)fgetc(file_pointer);
		if (current_char != ',')
			forest[i] = current_char;

	} while (file_pointer != EOF);
	return forest;
}