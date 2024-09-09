#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "sort.h"
#include "fs.h"
#include "io.h"

static const char* POEM_PATH = "onegin.txt";

int main()
{
	if(!load_poem(POEM_PATH))
	{
		fprintf(stderr, "Cant read poem!\n!");
		unload_poem();

		return -1;
	}
	
	print_strings((const char**)poem, poem_lines, "Poem unsorted");

	// random_sort(poem, poem_lines, ostrcmp);
	qsort(poem, poem_lines, sizeof(char*), str_comparator);

	print_strings((const char**)poem, poem_lines, "Poem sorted");

	unload_poem();
	return 0;
}
