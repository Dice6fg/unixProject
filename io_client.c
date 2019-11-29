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
	int i;
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
			p0_fifo = open("fifo0", O_RDONLY | O_NONBLOCK);
            p2_fifo = open("fifo2", O_RDONLY | O_NONBLOCK);
			fd = open("io0_64_result_client.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
		}
		else if(atoi(argv[1]) == 1)
		{
			p1_fifo = open("fifo5", O_RDONLY | O_NONBLOCK);			
			p3_fifo = open("fifo7", O_RDONLY | O_NONBLOCK);
			fd = open("io1_64_result_client.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
		}
	}
	else if(atoi(argv[2]) == 256)
	{
		if(atoi(argv[1]) == 0)
		{	
			p0_fifo = open("fifo0", O_RDONLY | O_NONBLOCK);
            p2_fifo = open("fifo2", O_RDONLY | O_NONBLOCK);
			fd = open("io0_256_result_client.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
		}
		else if(atoi(argv[1]) == 1)
		{
			p1_fifo = open("fifo5", O_RDONLY | O_NONBLOCK);			
			p3_fifo = open("fifo7", O_RDONLY | O_NONBLOCK);
			fd = open("io1_256_result_client.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
		}
	}
	

	if(atoi(argv[1]) == 0)
    {
        while(1)
        {
            if(b = read(p0_fifo, &buf[count], sizeof(int)) > 0)
            { 
                count++;
            }
            
            if(count == bufsz/sizeof(int)/2)
            break;
        }
        while(1)
        {
            if(b = read(p2_fifo, &buf[count], sizeof(int)) > 0)
            {                 
                count++;
            }
            
            if(count == bufsz/sizeof(int))
            break;
        }
        printf("buf load finished\n");
        close(p0_fifo);
        close(p2_fifo);
    }
    if(atoi(argv[1]) == 1)
    {
        while(1)
        {
            if(b = read(p1_fifo, &buf[count], sizeof(int)) > 0)
            {              
                count++;
            }
           
            if(count == bufsz/sizeof(int)/2)
            break;
        }
        while(1)
        {
           
            if(b = read(p3_fifo, &buf[count], sizeof(int)) > 0)
            {               
                count++;
            } 
            
            if(count == bufsz/sizeof(int))
            break;
        }
        printf("buf load finished\n");
        close(p1_fifo);        
        close(p3_fifo);
    }

	
	for(i=0; i<bufsz/sizeof(int); i++)
	{
		write(fd, &buf[i], sizeof(int));
	}	
	
	printf("write finished\n");
		
	close(fd);
	
	printf("io: %d,  bufsize: %d\n", atoi(argv[1]), bufsz);
	return 0;
}
