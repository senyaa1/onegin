#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

static void memswap(void* a, void* b, size_t len)
{
	void* buf = calloc(len, 1);

	memcpy(buf, b, len);
	memcpy(b, a, len);
	memcpy(a, buf, len);
	
	free(buf);
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

static bool is_punctuation(char c) 
{
	return strchr("!\"#$%&'()*+,./:;<=>?@[\\]^_`{|}~ «»", c) != NULL;
}

int str_comparator(const void* in_a, const void* in_b)
{
	char* a = *(char**)in_a;
	char* b = *(char**)in_b;

	size_t len_a = strlen(a), len_b = strlen(b);
	size_t min_len = ((len_a < len_b) ? len_a : len_b);

	size_t a_ptr = 0, b_ptr = 0;

	while(a_ptr < min_len && b_ptr < min_len)
	{
		while(is_punctuation(a[a_ptr]) && a_ptr < min_len)
		{
			a_ptr++;
		}

		while(is_punctuation(b[b_ptr]) && b_ptr < min_len)
		{
			b_ptr++;
		}

		if(a[a_ptr] == b[b_ptr])
		{
			a_ptr++;
			b_ptr++;
			continue;
		}

		return (a[a_ptr] < b[b_ptr]) ? -1 : 1;
	}
	
	return 0;
}
