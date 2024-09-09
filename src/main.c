#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>


void memswap(void* a, void* b, size_t len)
{
	// printf("swapping %s and %s\n", *(char**)a, *(char**)b);

	void* buf = calloc(len, 1);

	memcpy(buf, b, len);
	memcpy(b, a, len);
	memcpy(a, buf, len);
	
	free(buf);
}

int str_comparator(const void* in_a, const void* in_b)
{
	char* a = *(char**)in_a;
	char* b = *(char**)in_b;

	size_t len_a = strlen(a);
	size_t len_b = strlen(b);
	
	for(size_t i = 0; i < ((len_a < len_b) ? len_a : len_b); i++)
	{
		if(a[i] == b[i])
			continue;
		return (a[i] < b[i]) ? -1 : 1;
	}

	if (len_a != len_b)
		return (len_a < len_b) ? -1 : 1;

	return 0;
}


// best sorting algo
// in the best case works faster than any other sorting algorithm
// patent pending
void random_sort(char** ptr, size_t cnt, int(*comparator)(const char*, const char*))
{
	srand(time(NULL)); // the seed better be good for this one

	bool is_sorted = false;
	while(!is_sorted)
	{
		is_sorted = true;

		for(size_t i = 0; i < (cnt - 1); i++)
		{
			if(comparator(ptr[i], ptr[i+1]) == 1)
			{
				is_sorted = false;
				break;
			}
		}

		if(is_sorted) break;

		char** r1 = (ptr + (rand() % cnt));
		char** r2 = (ptr + (rand() % cnt));

		memswap(r1, r2, sizeof(char*));
	}
}

void print_strings(const char** arr, size_t size, const char* title)
{
	printf("%s: \n", title);

	for(size_t i = 0; i < size; i++)
	{
		printf("\t%s\n", arr[i]);
	}
}

static const size_t MAX_LINES = 10000;
static const size_t MAX_LINE_SZ = 256;

static char** poem = NULL;
static char* poem_data = NULL;
static size_t poem_lines = 0;


bool load_poem()
{
	int fd = open("obama.txt", O_RDONLY);
	if (fd == -1)
		return false;

	struct stat st = {0};
	fstat(fd, &st);
	off_t sz = st.st_size;

	void* fileptr = mmap(NULL, sz, PROT_READ, MAP_PRIVATE, fd, 0);

	poem = (char**)calloc(MAX_LINES, sizeof(char*));
	poem_data = (char*)calloc(MAX_LINES * MAX_LINE_SZ, sizeof(char));

	memcpy(poem_data, fileptr, sz);

	munmap(fileptr, sz);
	close(fd);

	size_t cur_len = 0;
	poem_lines = 0;
	for(size_t i = 0; i < sz; i++)
	{
		if(poem_data[i] != '\n')
		{
			cur_len++;
			continue;
		}

		poem_data[i] = '\0';
		poem[poem_lines++] = poem_data + (i - cur_len);
		cur_len = 0;
	}
	poem = (char**)realloc(poem, poem_lines * sizeof(char*));

	// printf("Initialized poem\t\t@ %p\n", poem);
	//
	// for(size_t i = 0; i < poem_lines; i++)
	// 	printf("    - %s\t\t@ %p\t @ %p\n", poem[i], poem[i], &poem[i]);
	//
	return true;
}

void unload_poem()
{
	free(poem_data);
	free(poem);
}


int main()
{
	if(!load_poem())
	{
		fprintf(stderr, "Cant read poem!\n!");
		return -1;
	}
	
	print_strings((const char**)poem, poem_lines, "Poem unsorted");

	// random_sort(poem, poem_lines, ostrcmp);
	qsort(poem, poem_lines, sizeof(char*), str_comparator);

	print_strings((const char**)poem, poem_lines, "Poem sorted");

	unload_poem();
	return 0;
}
