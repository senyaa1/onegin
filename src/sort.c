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

static inline bool is_punctuation(wchar_t c) 
{
	return wcschr(L"!\"#$%&'()*+,./:;<=>?@[\\]-—^_`{|}~ «»", c) != NULL;
}

static inline size_t partition(void *arr, size_t elsize, size_t l, size_t r, int(*comparator)(const void*, const void*)) 
{
	void* pivot = arr + r * elsize;
	size_t i = l - 1;

	for (size_t j = l; j < r; j++) {
		// if (arr[j] > pivot)
		if (comparator(arr + j * elsize, pivot) == 1)
			continue;

		memswap(arr + ++i * elsize, arr + j * elsize, sizeof(int));
	}

	memswap(arr + ++i * elsize, arr + r * elsize, sizeof(int));
	return i;
}

static void quicksort(void *arr, size_t elsize, int l, int r, int(*comparator)(const void*, const void*))
{
	if (l >= r)
		return;

        size_t pivot_index = partition(arr, elsize, l, r, comparator);

        quicksort(arr, elsize, l, pivot_index - 1, comparator);
        quicksort(arr, elsize, pivot_index + 1, r, comparator);
}

void quick_sort(void *array, size_t cnt, size_t elsize, int(*comparator)(const void*, const void*))
{
	quicksort(array, elsize, 0, cnt - 1, comparator);
}

// best sorting algo
// in the best case works faster than any other sorting algorithm
// patent pending
void random_sort(void* array, size_t cnt, size_t elsize, int(*comparator)(const void*, const void*))
{
	srand(time(NULL)); // the seed better be good for this one

	bool is_sorted = false;
	while(!is_sorted)
	{
		is_sorted = true;

		for(size_t i = 0; i < (cnt - 1); i++)
		{
			if(comparator(array + i * elsize, array + (i+1) * elsize) == 1)
			{
				is_sorted = false;
				break;
			}
		}

		if(is_sorted) break;

		void* r1 = (array + (rand() % cnt) * elsize);
		void* r2 = (array + (rand() % cnt) * elsize);

		memswap(r1, r2, elsize);
	}
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
