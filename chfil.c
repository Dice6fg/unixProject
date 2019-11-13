#include <stdio.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
	int fd;
	int num;

	fd = open(argv[1], O_RDONLY);
	while(read(fd, &num, sizeof(int)) > 0)
		printf("%d\n", num);

	return 0;
}
