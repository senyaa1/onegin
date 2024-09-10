#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <wchar.h>
#include <wctype.h>

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
void random_sort(wchar_t** ptr, size_t cnt, int(*comparator)(const wchar_t*, const wchar_t*))
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

		wchar_t** r1 = (ptr + (rand() % cnt));
		wchar_t** r2 = (ptr + (rand() % cnt));

		memswap(r1, r2, sizeof(wchar_t*));
	}
}

static bool is_punctuation(wchar_t c) 
{
	return wcschr(L"!\"#$%&'()*+,./:;<=>?@[\\]-—^_`{|}~ «»", c) != NULL;
}

int str_comparator(const void* in_a, const void* in_b)
{
	wchar_t* a = *(wchar_t**)in_a;
	wchar_t* b = *(wchar_t**)in_b;

	size_t len_a = wcslen(a), len_b = wcslen(b);
	size_t min_len = ((len_a < len_b) ? len_a : len_b);
	size_t a_ptr = 0, b_ptr = 0;

	while(a_ptr < min_len && b_ptr < min_len)
	{
		while(is_punctuation(a[a_ptr]) && a_ptr < min_len)
			a_ptr++;

		while(is_punctuation(b[b_ptr]) && b_ptr < min_len)
			b_ptr++;

		wchar_t chr_a = towlower(a[a_ptr]), chr_b = towlower(b[b_ptr]);

		if(chr_a == chr_b)
		{
			a_ptr++;
			b_ptr++;
			continue;
		}

		return (chr_a < chr_b) ? -1 : 1;
	}
	return 0;
}
