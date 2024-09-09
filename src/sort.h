#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void random_sort(char** ptr, size_t cnt, int(*comparator)(const char*, const char*));
int str_comparator(const void* in_a, const void* in_b);

