#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>


int main()
{
	int fd = open("onegin.txt", O_RDONLY);

	close(fd);
	return 0;
}
