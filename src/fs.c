#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "fs.h"
#include "ptrs.h"

size_t poem_lines = 0;
wchar_t** poem = NULL;
wchar_t* poem_data = NULL;

static const size_t MAX_LINES = 10000;


bool load_poem(const char* poem_path)
{
	int fd = open(poem_path, O_RDONLY);
	if (fd == -1)
		return false;

	struct stat st = {0};

	fstat(fd, &st);
	off_t sz = st.st_size;

	char* fileptr = (char*)mmap(NULL, sz, PROT_READ, MAP_PRIVATE, fd, 0);

	poem = (wchar_t**)calloc(MAX_LINES, sizeof(wchar_t*));
	poem_data = (wchar_t*)calloc(sz + 1, sizeof(wchar_t));
	if(!poem || !poem_data || !fileptr)
	{
		fprintf(stderr, "Memory allocation error!\n");
		return false;
	}
	size_t wchar_len = mbstowcs(poem_data, fileptr, sz);

	wchar_t* reallocated_poem_data = (wchar_t*)realloc(poem_data, wchar_len * sizeof(wchar_t));
	if(!reallocated_poem_data)
	{
		fprintf(stderr, "Memory allocation error!\n");
		return false;
	}
	poem_data = reallocated_poem_data;

	munmap(fileptr, sz);
	close(fd);

	size_t cur_len = 0;
	poem_lines = 0;
	for(size_t i = 0; i < wchar_len; i++)
	{
		if(poem_data[i] != L'\n')
		{
			cur_len++;
			continue;
		}
		poem_data[i] = L'\0';

		poem[poem_lines++] = PTR_PACK((poem_data + (i - cur_len)), cur_len);
		cur_len = 0;
	}

	wchar_t** realloced_poem = (wchar_t**)realloc(poem, poem_lines * sizeof(wchar_t*));
	if(!realloced_poem)
	{
		fprintf(stderr, "Memory allocation error!\n");
		return false;
	}
	poem = realloced_poem;

	return true;
}

void unload_poem()
{
	free(poem_data);
	free(poem);
}

