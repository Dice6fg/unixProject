#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
	int p0_fifo, p1_fifo, p2_fifo, p3_fifo;	
	int fd;	

	int* buf;
	int bufsz;

	int count = 0;
	int i,j,a;
	int zero = 0;

	size_t b = 0;

	count = 0;
	if(atoi(argv[2]) == 64)
		bufsz = 8000*sizeof(int);
	else if(atoi(argv[2]) == 256)
		bufsz = 32000*sizeof(int);

	buf = (int*)malloc(bufsz);
	


	if(atoi(argv[2]) == 64)
	{
		if(atoi(argv[1]) == 0)
		{	
			p0_fifo = open("fifo0", O_RDONLY);
			p1_fifo = open("fifo1", O_RDONLY);
			p2_fifo = open("fifo2", O_RDONLY);
			p3_fifo = open("fifo3", O_RDONLY);
			fd = open("io0_64_result_c.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
		}
		else if(atoi(argv[1]) == 1)
		{
			p0_fifo = open("fifo4", O_RDONLY);
			p1_fifo = open("fifo5", O_RDONLY);
			p2_fifo = open("fifo6", O_RDONLY);
			p3_fifo = open("fifo7", O_RDONLY);
			fd = open("io1_64_result_c.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
		}
	}
	else if(atoi(argv[2]) == 256)
	{
		if(atoi(argv[1]) == 0)
		{	
			p0_fifo = open("fifo0", O_RDONLY);
			p1_fifo = open("fifo1", O_RDONLY);
			p2_fifo = open("fifo2", O_RDONLY);
			p3_fifo = open("fifo3", O_RDONLY);
			fd = open("io0_256_result_c.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
		}
		else if(atoi(argv[1]) == 1)
		{
			p0_fifo = open("fifo4", O_RDONLY);
			p1_fifo = open("fifo5", O_RDONLY);
			p2_fifo = open("fifo6", O_RDONLY);
			p3_fifo = open("fifo7", O_RDONLY);
			fd = open("io1_256_result_c.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
		}
	}
	

	
	while(1)
	{
		if(b = read(p0_fifo, &buf[count], sizeof(int)) > 0)
		{ 
			printf("%d bytes read\n", b);
			printf("p0_fifo count: %d\n", count);
			printf("buf[%d]: %d\n", count, buf[count]);
			count++;
		}
		if(b = read(p1_fifo, &buf[count], sizeof(int)) > 0)
		{
			printf("%d bytes read\n", b);
			printf("p1_fifo count: %d\n", count);
			printf("buf[%d]: %d\n", count, buf[count]);
			count++;
		}
		if(b = read(p2_fifo, &buf[count], sizeof(int)) > 0)
		{ 
			printf("%d bytes read\n", b);
			printf("p2_fifo count: %d\n", count);
			printf("buf[%d]: %d\n", count, buf[count]);
			count++;
		}
		if(b = read(p3_fifo, &buf[count], sizeof(int)) > 0)
		{
			printf("%d bytes read\n", b);
			printf("p3_fifo count: %d\n", count);
			printf("buf[%d]: %d\n", count, buf[count]);
			count++;
		} 
		
		if(count == bufsz/sizeof(int))
			break;
	}
	printf("buf load finished\n");

	if(atoi(argv[2]) ==64){
		if(atoi(argv[1]) == 0){
				for(i=0; i<bufsz/sizeof(int); i++)
				{
					a = buf[i]/10;
					if((a>= 0  && a < 1000) || (a>= 2000  && a < 3000) || (a>= 4000  && a < 5000) || (a>= 6000  && a < 7000) || (a>= 8000  && a < 9000) || (a>= 10000  && a < 11000) || (a>= 12000  && a < 13000) || (a>= 14000  && a < 15000))
					{
						j = a/2000;
						lseek(fd, (a-1000*j)*sizeof(int), SEEK_SET);
						write(fd, &buf[i], sizeof(int));
					}
				}
		}
		else if(atoi(argv[1]) == 1){
				for(i=0; i<bufsz/sizeof(int); i++)
				{
					a = buf[i]/10;
				
		if((a>= 1000  && a < 2000) || (a>= 3000  && a < 4000) || (a>= 5000  && a < 6000) || (a>= 7000  && a < 8000) || (a>= 9000  && a < 10000) || (a>= 11000  && a < 12000) || (a>= 13000  && a < 14000) || (a>= 15000  && a < 16000))
					{
						j = a/2000;
						lseek(fd, (a-1000*(j+1))*sizeof(int), SEEK_SET);
						write(fd, &buf[i], sizeof(int));
					}
				}
			}
	
	}
	if(atoi(argv[2]) == 256){
		if(atoi(argv[1]) == 0){
				for(i=0; i<bufsz/sizeof(int); i++)
				{
					a = buf[i]/10;
					j = a/2000;
					if(a>= (0 + 2000*j) && a < (1000 + 2000*j))
					{
						lseek(fd, (a-1000*j)*sizeof(int), SEEK_SET);
						write(fd, &buf[i], sizeof(int));
					}
				}
			}
		else if(atoi(argv[1]) == 1){
				for(i=0; i<bufsz/sizeof(int); i++)
				{
					a = buf[i]/10;
					j = a/2000;
					if(a>=(1000 + 2000*j) && a<(2000 + 2000*j))
					{
						lseek(fd, (a-1000*(j+1))*sizeof(int), SEEK_SET);
						write(fd, &buf[i], sizeof(int));
					}
				}
			}
	
	}
	
	printf("write finished\n");
	
	close(p0_fifo);
	close(p1_fifo);
	close(p2_fifo);
	close(p3_fifo);
	
	close(fd);
	
	printf("io: %d,  bufsize: %d\n", atoi(argv[1]), bufsz);
	return 0;
}
