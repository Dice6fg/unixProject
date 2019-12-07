#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
	int p0_fifo, p1_fifo, p2_fifo, p3_fifo;	
	int fd;	

	int* buf;
	int temp1[8][1000];
	int temp2[32][1000];
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
			p0_fifo = open("fifo0", O_RDWR );
			p1_fifo = open("fifo1", O_RDWR );
			p2_fifo = open("fifo2", O_RDWR );
			p3_fifo = open("fifo3", O_RDWR );
			fd = open("io0_64_result_c.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
		}
		else if(atoi(argv[1]) == 1)
		{
			p0_fifo = open("fifo4", O_RDWR);
			p1_fifo = open("fifo5", O_RDWR);
			p2_fifo = open("fifo6", O_RDWR);
			p3_fifo = open("fifo7", O_RDWR );
			fd = open("io1_64_result_c.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
		}
	}
	else if(atoi(argv[2]) == 256)
	{
		if(atoi(argv[1]) == 0)
		{	
			p0_fifo = open("fifo0", O_RDONLY );
			p1_fifo = open("fifo1", O_RDONLY );
			p2_fifo = open("fifo2", O_RDONLY );
			p3_fifo = open("fifo3", O_RDONLY );
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
	

	if(atoi(argv[2]) == 64)
	{
		
			read(p0_fifo, temp1[0], sizeof(int) * 1000);	
			read(p0_fifo, temp1[1], sizeof(int) * 1000);		
			read(p1_fifo, temp1[2], sizeof(int) * 1000);		
			read(p1_fifo, temp1[3], sizeof(int) * 1000);			
			read(p2_fifo, temp1[4], sizeof(int) * 1000); 		
			read(p2_fifo, temp1[5], sizeof(int) * 1000); 		
			read(p3_fifo, temp1[6], sizeof(int) * 1000);			
			read(p3_fifo, temp1[7], sizeof(int) * 1000); 

			printf("buf load finished\n");
			for(i = 0; i < 8; i++) 
			{
				printf("%d : re : %d\n",i, temp1[i][1]);
				b = write(fd, temp1[i], sizeof(int) * 1000);
				printf("w =  %d\n", b);
			}
			printf("write finished\n");
					
	}
	else if(atoi(argv[2]) == 256)
	{
		read(p0_fifo, temp2[0], sizeof(int) * 1000);	
		read(p0_fifo, temp2[1], sizeof(int) * 1000);		
		read(p0_fifo, temp2[2], sizeof(int) * 1000);		
		read(p0_fifo, temp2[3], sizeof(int) * 1000);			
		read(p0_fifo, temp2[4], sizeof(int) * 1000); 		
		read(p0_fifo, temp2[5], sizeof(int) * 1000); 		
		read(p0_fifo, temp2[6], sizeof(int) * 1000);			
		read(p0_fifo, temp2[7], sizeof(int) * 1000); 

		read(p1_fifo, temp2[8], sizeof(int) * 1000);	
		read(p1_fifo, temp2[9], sizeof(int) * 1000);		
		read(p1_fifo, temp2[10], sizeof(int) * 1000);		
		read(p1_fifo, temp2[11], sizeof(int) * 1000);			
		read(p1_fifo, temp2[12], sizeof(int) * 1000); 		
		read(p1_fifo, temp2[13], sizeof(int) * 1000); 		
		read(p1_fifo, temp2[14], sizeof(int) * 1000);			
		read(p1_fifo, temp2[15], sizeof(int) * 1000); 

		read(p2_fifo, temp2[16], sizeof(int) * 1000);	
		read(p2_fifo, temp2[17], sizeof(int) * 1000);		
		read(p2_fifo, temp2[18], sizeof(int) * 1000);		
		read(p2_fifo, temp2[19], sizeof(int) * 1000);			
		read(p2_fifo, temp2[20], sizeof(int) * 1000); 		
		read(p2_fifo, temp2[21], sizeof(int) * 1000); 		
		read(p2_fifo, temp2[22], sizeof(int) * 1000);			
		read(p2_fifo, temp2[23], sizeof(int) * 1000); 

		read(p3_fifo, temp2[24], sizeof(int) * 1000);	
		read(p3_fifo, temp2[25], sizeof(int) * 1000);		
		read(p3_fifo, temp2[26], sizeof(int) * 1000);		
		read(p3_fifo, temp2[27], sizeof(int) * 1000);			
		read(p3_fifo, temp2[28], sizeof(int) * 1000); 		
		read(p3_fifo, temp2[29], sizeof(int) * 1000); 		
		read(p3_fifo, temp2[30], sizeof(int) * 1000);			
		read(p3_fifo, temp2[31], sizeof(int) * 1000); 

		printf("buf load finished\n");
			for(i = 0; i < 32; i++) 
			{
				printf("%d : re : %d\n",i, temp2[i][1]);
				b = write(fd, temp2[i], sizeof(int) * 1000);
				printf("w =  %d\n", b);
			}
			printf("write finished\n");
	}
		
		
	
	close(p0_fifo);
	close(p1_fifo);
	close(p2_fifo);
	close(p3_fifo);
	
	close(fd);
	
	printf("io: %d,  bufsize: %d\n", atoi(argv[1]), bufsz);
	return 0;
}
