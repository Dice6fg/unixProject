#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<malloc.h>

int main(int argc, char *argv[])
{

	int fd, fd1,fd2,fd3,fd4;
	int i,j;
	int rfd1[1000];
	int rfd2[1000];
	int ptr1[4][1000];
	int ptr2[16][1000];
	int io0, io1;
	char* fifoname;
	int bufsiz;

	int w = 0;
	
	if (atoi(argv[2]) == 64) 
	{
		bufsiz = 1000;	
		fd = open("data1.txt", O_RDONLY);
		switch (atoi(argv[1]))
		{
			case 0: 
				fd1 = open("computenode1_64.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
				io0 = open("fifo0", O_WRONLY);
				io1 = open("fifo4", O_WRONLY);

				for (i = 0; i < 4; i++)
				{
					lseek(fd, sizeof(int)*bufsiz*2*i, SEEK_SET);

					read(fd, rfd1, sizeof(rfd1));

					for (j = 0; j < bufsiz; j++)
					{
						ptr1[i][j] = rfd1[j];
					}

					write(fd1, rfd1, sizeof(rfd1));
				}

				for(i = 0; i < 4;i++)
				{
					for(j = 0; j < bufsiz; j ++)
					{
						w = write(io0, &ptr1[i][j], sizeof(int));
					}
				}

				close(fd1);
				close(io0);
				close(io1);
				break;
			case 1: 
				fd1 = open("computenode2_64.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
				io0 = open("fifo1", O_WRONLY);
				io1 = open("fifo5", O_WRONLY);
				for (i = 0; i < 4; i++)
				{
					lseek(fd, sizeof(int)*bufsiz*((2*i)+1) , SEEK_SET);
					read(fd, rfd1, sizeof(rfd1));
					for (j = 0; j < bufsiz; j++)
					{
						ptr1[i][j] = rfd1[j];
					}
					write(fd1, rfd1, sizeof(rfd1));
					
				}

				for(i=0;i<4;i++)
				{
					for(j=0; j<bufsiz; j++)
					{
						w = write(io1, &ptr1[i][j], sizeof(int));
					}
				}

				close(fd1);
				close(io0);
				close(io1); 
				break;
			case 2:
				fd1 = open("computenode3_64.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
				io0 = open("fifo2", O_WRONLY);
				io1 = open("fifo6", O_WRONLY);
				for (i = 0; i < 4; i++)
				{
					lseek(fd, sizeof(int) * bufsiz * ((2 * i) + 8), SEEK_SET);
					read(fd, rfd1, sizeof(rfd1));
					for (j = 0; j < bufsiz; j++)
					{
						ptr1[i][j] = rfd1[j];
					}
					write(fd1, rfd1, sizeof(rfd1));

				}
				for(i=0;i<4;i++)
				{
					for(j=0; j<bufsiz; j++)
					{
						w = write(io0, &ptr1[i][j], sizeof(int));
					}
				}

				close(fd1);
				close(io0);
				close(io1); 
				break;
			case 3:
				io0 = open("fifo3", O_WRONLY);
				io1 = open("fifo7", O_WRONLY);
				fd1 = open("computenode4_64.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
				for (i = 0; i < 4; i++)
				{
					lseek(fd, sizeof(int) * bufsiz * ((2 * i) + 9), SEEK_SET);
					read(fd, rfd1, sizeof(rfd1));
					for (j = 0; j < bufsiz; j++)
					{
						ptr1[i][j] = rfd1[j];
					}
					write(fd1, rfd1, sizeof(rfd1));

				}

				for(i=0;i<4;i++)
				{
					for(j=0; j<bufsiz; j++)
					{
						w = write(io1, &ptr1[i][j], sizeof(int));
					}
				}

				close(fd1);
				close(io0);
				close(io1);
				break;

			default: break;
		}
		close(fd);
	}
	else if(atoi(argv[2]) == 256) 
	{
		bufsiz = 1000;		
		fd = open("data2.txt", O_RDONLY);

		switch (atoi(argv[1]))
		{
		case 0:
			io0 = open("fifo0", O_WRONLY);
			io1 = open("fifo4", O_WRONLY);
			fd1 = open("computenode1_256.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
			for (i = 0; i < 16; i++)
			{
				lseek(fd, sizeof(int) * bufsiz * (2*i), SEEK_SET);
				read(fd, rfd2, sizeof(rfd2));
				for (j = 0; j < bufsiz; j++)
				{
					ptr2[i][j] = rfd2[j];
				}
				write(fd1, rfd2, sizeof(rfd2));
			
			}
			for(i=0;i<16;i++)
			{
				for(j=0; j<bufsiz; j++)
				{
					write(io0, &ptr2[i][j], sizeof(int));
				}
			}
                       
                	close(fd1);
			close(io0);
			close(io1);
			break;
		case 1:
			io0 = open("fifo1", O_WRONLY);
			io1 = open("fifo5", O_WRONLY);
			fd1 = open("computenode2_256.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
			for (i = 0; i < 16; i++)
			{
				lseek(fd, sizeof(int) * bufsiz * ((2*i) + 1), SEEK_SET);
				read(fd, rfd2, sizeof(rfd2));
				for (j = 0; j < bufsiz; j++)
				{
					ptr2[i][j] = rfd2[j];
				}
				write(fd1, rfd2, sizeof(rfd2));

			}

			for(i=0;i<16;i++)
			{
				for(j=0; j<bufsiz; j++)
				{
					write(io1, &ptr2[i][j], sizeof(int));
				}
			}
			
			close(fd1);
			close(io0);
			close(io1);
			break;
		case 2:
			io0 = open("fifo2", O_WRONLY);
			io1 = open("fifo6", O_WRONLY);
			fd1 = open("computenode3_256.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
			for (i = 0; i < 16; i++)
			{
				lseek(fd, sizeof(int) * bufsiz * ((2*i) + 32), SEEK_SET);
				read(fd, rfd2, sizeof(rfd2));
				for (j = 0; j < bufsiz; j++)
				{
					ptr2[i][j] = rfd2[j];
				}
				write(fd1, rfd2, sizeof(rfd2));

			}
			
			for(i=0;i<16;i++)
			{
				for(j=0; j<bufsiz; j++)
				{
					write(io0, &ptr2[i][j], sizeof(int));
				}
			}
			
			close(fd1);
			close(io0);
			close(io1);
			break;
		case 3:
			io0 = open("fifo3", O_WRONLY);
			io1 = open("fifo7", O_WRONLY);
			fd1 = open("computenode4_256.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
			for (i = 0; i < 16; i++)
			{
				lseek(fd, sizeof(int) * bufsiz * ((2*i) + 33), SEEK_SET);
				read(fd, rfd2, sizeof(rfd2));
				for (j = 0; j < bufsiz; j++)
				{
					ptr2[i][j] = rfd2[j];
				}
				write(fd1, rfd2, sizeof(rfd2));

			}

			for(i=0;i<16;i++)
			{
				for(j=0; j<bufsiz; j++)
				{
					write(io1, &ptr2[i][j], sizeof(int));
				}
			}

			close(fd1);
			close(io0);
			close(io1);
			break;
		default: break;
		}
		close(fd);
	}
}
