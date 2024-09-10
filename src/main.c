#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <locale.h>
#include <wchar.h>

#include "sort.h"
#include "fs.h"
#include "io.h"

static const size_t LINES_PER_SYM = 2;
static const wchar_t* ALPH = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЮЯ";

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
	// random_sort(poem, poem_lines, ostrcmp);
	qsort(poem, poem_lines, sizeof(char*), str_comparator);

	// print 2 per line in alph order
	size_t cursym = 0, printed = 0;
	for(size_t line = 0; line < poem_lines; line++)
	{
		if(poem[line][0] < ALPH[cursym])
			continue;

		printf("\t%ls\n", poem[line]);

		if(++printed == LINES_PER_SYM)
		{
			cursym++;
			printed = 0;
		}

		if(cursym >= 28)
			break;

	}

	// print_strings((const char**)poem, 15, "Sorted Onegin:");

	unload_poem();
	return 0;
}
