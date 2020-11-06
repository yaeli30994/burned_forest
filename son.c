#include "son.h"

int main(int argc, char* argv[])
{
	//check if there are sufficiante amount of arguments
	if (argc == 2) 
		printf("The string (forest) supplied is %s\n", argv[1]);
	else if (argc > 2)
		printf("Too many arguments supplied.\n");
	else
		printf("One argument expected (program's name).\n");
	exit(get_num_of_cell_type(argv[1], 'F'));
	
	//for father- check what to do it the input is not a square
	//int s = sqrt(forrest_size);
	//if ((s * s) == forrest_s[i]) //if true- they forest is square
}


int get_num_of_cell_type(char *forest, char cell_type)
{
	int counter = 0, i = 0;
    char *pointer = forest;
    while (pointer[i] != NULL)
    {
        if (pointer[i] == cell_type)
            counter++;
        i++;
    }
    return counter;
}
