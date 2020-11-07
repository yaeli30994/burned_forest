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
	printf("%d %d %d", forest_size, num_of_gens, num_of_trees);
	char* str = get_forest(num_of_trees, file_pointer);
	printf("%s", str);

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