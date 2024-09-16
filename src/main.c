#include <stdio.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <locale.h>
#include <wchar.h>

#include "sort.h"
#include "fs.h"
#include "io.h"


int main(int argc, char** argv)
{
	setlocale(LC_ALL, "ru_RU.UTF-8");

	if(argc != 2)
	{
		fprintf(stderr, "Provide path to the poem!\n");
		return -1;
	}

	if(!load_poem(argv[1]))
	{
		fprintf(stderr, "Can't read poem!\n");
		return -1;
	}
	
	// print_strings((const wchar_t**)poem, poem_lines, "UnSorted Onegin:");
	
	// qsort(poem, poem_lines, sizeof(char*), str_comparator);
	quick_sort(poem, poem_lines, sizeof(char*), str_comparator);
	// random_sort(poem, poem_lines, sizeof(char*), str_comparator);

	print_strings((const wchar_t**)poem, 15, "Sorted Onegin:");

	unload_poem();
	return 0;
}
