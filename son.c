#include "son.h"

int main(int argc, char* argv[])
{
	return get_burned_trees_num(argv[1], 'F');
}


int get_burned_trees_num(char *string, char value)
{
	int counter = 0, i = 0;
    char *pointer = string;
    while (pointer[i] != NULL)
    {
        if (pointer[i] == value)
            counter++;
        i++;
    }
    return counter;
}