#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argv, char* argv[])
{
	int p0_fifo, p1_fifo1, p2_fifo, p3_fifo;	
	int fd;	

	int* buf;
	int bufsz;

	int count;
	int i;

	p0_fifo = open("fifo0", O_RDONLY | O_NONBLOCK);
	p1_fifo = open("fifo1", O_RDONLY | O_NONBLOCK);
	p2_fifo = open("fifo2", O_RDONLY | O_NONBLOCK);
	p3_fifo = open("fifo3", O_RDONLY | O_NONBLOCK);
	
	fd = open("result.txt", O_WRONLY);

	count = 0;
	bufsz = atoi(argv[2])*1000/2;
	buf = (int*)malloc(bufsz);
	
	while(true)
	{
		if(read(p0_fifo, &buf[count], sizeof(int)) > 0)
			count++; 
		if(read(p1_fifo, &buf[count], sizeof(int)) > 0)
			count++; 
		if(read(p2_fifo, &buf[count], sizeof(int)) > 0)
			count++; 
		if(read(p3_fifo, &buf[count], sizeof(int)) > 0)
			count++; 
		
		if(count == bufsz)
			break;
	}

	for(i=0; i<bufsz; i++)
	{
		lseek(fd, buf[i]/10, SEEK_SET);
		write(fd, &buf[i], sizeof(int));
	}	

	close(p0_fifo);
	close(p1_fifo);
	close(p2_fifo);
	close(p3_fifo);
	
	close(fd);

	return 0;
}
