#include <stdio.h>
#include <fcntl.h>

int main(void)
{
	int fd1, fd2, i;
	int buf;
	fd1 = open("data1.txt", O_CREAT | O_TRUNC | O_RDWR);
	fd2 = open("data2.txt", O_CREAT | O_TRUNC | O_RDWR);
	
	for(i=0; i<160000; i++)
	{
		buf = i*10;
		write(fd1, &buf, sizeof(int));
	}

	for(i=0; i<640000; i++)
	{
		buf = i*10;
		write(fd2, &buf, sizeof(int));
	}

	lseek(fd1, -1, SEEK_END);
	read(fd1, &buf, sizeof(int));
	printf("test: %d\n", buf);

	close(fd1);
	close(fd2);
	return 0;
}
