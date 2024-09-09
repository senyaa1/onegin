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

size_t poem_lines = 0;
char** poem = NULL;

static const size_t MAX_LINES = 10000;
static const size_t MAX_LINE_SZ = 256;
static char* poem_data = NULL;

bool load_poem(const char* poem_path)
{
	int fd = open(poem_path, O_RDONLY);
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

	// convert newlines to str terminators and store pointers to them 
	// we can also store strlen of the string in the MSBs of the pointers, so that strlens won't recompute on sort
	// but for now we leave it as is
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

	return true;
}

void unload_poem()
{
	free(poem_data);
	free(poem);
}

