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
wchar_t** poem = NULL;

static const size_t MAX_LINES = 10000;
static const size_t MAX_LINE_SZ = 256;
static wchar_t* poem_data = NULL;

bool load_poem(const char* poem_path)
{
	int fd = open(poem_path, O_RDONLY);
	if (fd == -1)
		return false;

	struct stat st = {0};
	fstat(fd, &st);
	off_t sz = st.st_size;

	void* fileptr = mmap(NULL, sz, PROT_READ, MAP_PRIVATE, fd, 0);

	poem = (wchar_t**)calloc(MAX_LINES, sizeof(wchar_t*));
	poem_data = (wchar_t*)calloc(sz, sizeof(wchar_t));

	char* poem_tmp_buf = (char*)calloc(sz, sizeof(char));

	memcpy(poem_tmp_buf, fileptr, sz);
	size_t wchar_len = mbstowcs(poem_data, poem_tmp_buf, sz);
	poem_data = (wchar_t*)realloc(poem_data, wchar_len * sizeof(wchar_t));

	munmap(fileptr, sz);
	close(fd);
	free(poem_tmp_buf);

	// convert newlines to str terminators and store pointers to them 
	// we can also store strlen of the string in the MSBs of the pointers, so that strlens won't recompute on sort
	// but for now we leave it as is
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
		poem[poem_lines++] = poem_data + (i - cur_len);
		cur_len = 0;
	}

	poem = (wchar_t**)realloc(poem, poem_lines * sizeof(wchar_t*));

	return true;
}

void unload_poem()
{
	free(poem_data);
	free(poem);
}

