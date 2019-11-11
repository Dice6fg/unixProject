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

	int fd, fd1;
	int *ptr;
	int i,j;
	int* rfd;
	int io0, io1;
	char* fifoname;
	int bufsiz;
	/*for (i = 0; i < sizeof(int) * 4000; i++)
 * 	{
 * 			write(io0, ptr, sizeof(int));
 * 				}
 *
 * 					close(io0);
 * 						close(io1);*/
	if (atoi(argv[2]) == 64) {
		bufsiz = 1000;
		rfd = (int*)malloc(sizeof(int) * bufsiz);
		ptr = malloc(sizeof(int) * 4 *bufsiz);
		fd = open("data1.txt", O_RDONLY);
		switch (atoi(argv[1]))
		{
			case 0: 
				io0 = open("fifo0", O_WRONLY);
				io1 = open("fifo4", O_WRONLY);
				fd1 = open("computenode1_64.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
				for (i = 0; i < 4; i++)
				{
					lseek(fd, sizeof(int) * bufsiz * 2 * i, SEEK_SET);
					read(fd, rfd, sizeof(rfd));
					for (j = 0; j < bufsiz; j++)
					{
						ptr[j+i*bufsiz] = rfd[j];
					}
					write(fd1, rfd, sizeof(rfd));
					
				}
				for(j=0;j<2;j++){
					for(i=0;i<bufsiz;i++)
					{
						write(io0,ptr,sizeof(int));
						execlp("io","io","0","64",(char*)NULL);
					}	
					for(i=0;i<bufsiz;i++)
					{
						write(io1,ptr,sizeof(int));
						execlp("io","io","1","64",(char*)NULL);
					}
				}
				close(fd1);
				close(io0);
				close(io1);
				break;
			case 1: 
				io0 = open("fifo1", O_WRONLY);
				io1 = open("fifo5", O_WRONLY);
				fd1 = open("computenode2_64.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
				for (i = 0; i < 4; i++)
				{
					lseek(fd, sizeof(int) * bufsiz * (2 * i + 1) , SEEK_SET);
					read(fd, rfd, sizeof(rfd));
					for (j = 0; j < bufsiz; j++)
					{
						ptr[j + i * bufsiz] = rfd[j];
					}
					write(fd1, rfd, sizeof(rfd));
					
				}
				for(j=0;j<2;j++){
                                        for(i=0;i<bufsiz;i++)
                                        {
                                                write(io0,ptr,sizeof(int));
						execlp("io","io","0","64",(char*)NULL);
                                        }
                                        for(i=0;i<bufsiz;i++)
                                        {
                                                write(io1,ptr,sizeof(int));	
						execlp("io","io","1","64",(char*)NULL);
                                        }
                                }
				close(fd1);
				close(io0);
				close(io1); 
				break;
			case 2:
				io0 = open("fifo2", O_WRONLY);
				io1 = open("fifo6", O_WRONLY);
				fd1 = open("computenode3_64.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
				for (i = 0; i < 4; i++)
				{
					lseek(fd, sizeof(int) * bufsiz * (2 * i + 8), SEEK_SET);
					read(fd, rfd, sizeof(rfd));
					for (j = 0; j < bufsiz; j++)
					{
						ptr[j + i * bufsiz] = rfd[j];
					}
					write(fd1, rfd, sizeof(rfd));

				}
				for(j=0;j<2;j++){
                                        for(i=0;i<bufsiz;i++)
                                        {
                                                write(io0,ptr,sizeof(int));
						execlp("io","io","0","64",(char*)NULL);
                                        }
                                        for(i=0;i<bufsiz;i++)
                                        {
                                                write(io1,ptr,sizeof(int));
						execlp("io","io","1","64",(char*)NULL);
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
					lseek(fd, sizeof(int) * bufsiz * (2 * i + 9), SEEK_SET);
					read(fd, rfd, sizeof(rfd));
					for (j = 0; j < bufsiz; j++)
					{
						ptr[j + i * bufsiz] = rfd[j];
					}
					write(fd1, rfd, sizeof(rfd));

				}
				for(j=0;j<2;j++){
                                        for(i=0;i<bufsiz;i++)
                                        {
                                                write(io0,ptr,sizeof(int));
						execlp("io","io","0","64",(char*)NULL);
                                        }
                                        for(i=0;i<bufsiz;i++)
                                        {
                                                write(io1,ptr,sizeof(int));
						execlp("io","io","1","64",(char*)NULL);
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
	
	else if(atoi(argv[2]) == 256) {
		bufsiz = 4000;
		rfd = (int*)malloc(sizeof(int) * bufsiz);
		ptr = malloc(sizeof(int) * 4 * bufsiz);
		fd = open("data2.txt", O_RDONLY);
		switch (atoi(argv[1]))
		{
		case 0:
			io0 = open("fifo0", O_WRONLY);
			io1 = open("fifo4", O_WRONLY);
			fd1 = open("computenode1_256.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
			for (i = 0; i < 4; i++)
			{
				lseek(fd, sizeof(int) * bufsiz * 2 * i, SEEK_SET);
				read(fd, rfd, sizeof(rfd));
				for (j = 0; j < bufsiz; j++)
				{
					ptr[j + i * bufsiz] = rfd[j];
				}
				write(fd1, rfd, sizeof(rfd));

			}
			for(j=0;j<2;j++){
                             for(i=0;i<bufsiz;i++)
                             {
				write(io0,ptr,sizeof(int));
				execlp("io","io","0","256",(char*)NULL);
                             }
			     for(i=0;i<bufsiz;i++)
			     {
				write(io1,ptr,sizeof(int));
				execlp("io","io","1","256",(char*)NULL);
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
			for (i = 0; i < 4; i++)
			{
				lseek(fd, sizeof(int) * bufsiz * (2 * i + 1), SEEK_SET);
				read(fd, rfd, sizeof(rfd));
				for (j = 0; j < bufsiz; j++)
				{
					ptr[j + i * bufsiz] = rfd[j];
				}
				write(fd1, rfd, sizeof(rfd));

			}
				for(j=0;j<2;j++){
					for(i=0;i<bufsiz;i++)
					{
						write(io0,ptr,sizeof(int));
						execlp("io","io","0","256",(char*)NULL);
					}	
					for(i=0;i<bufsiz;i++)
					{
						write(io1,ptr,sizeof(int));
						execlp("io","io","1","256",(char*)NULL);
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
			for (i = 0; i < 4; i++)
			{
				lseek(fd, sizeof(int) * bufsiz * (2 * i + 8), SEEK_SET);
				read(fd, rfd, sizeof(rfd));
				for (j = 0; j < bufsiz; j++)
				{
					ptr[j + i * bufsiz] = rfd[j];
				}
				write(fd1, rfd, sizeof(rfd));

			}
				for(j=0;j<2;j++){
					for(i=0;i<bufsiz;i++)
					{
						write(io0,ptr,sizeof(int));
						execlp("io","io","0","256",(char*)NULL);
					}	
					for(i=0;i<bufsiz;i++)
					{
						write(io1,ptr,sizeof(int));
						execlp("io","io","1","256",(char*)NULL);
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
			for (i = 0; i < 4; i++)
			{
				lseek(fd, sizeof(int) * bufsiz * (2 * i + 9), SEEK_SET);
				read(fd, rfd, sizeof(rfd));
				for (j = 0; j < bufsiz; j++)
				{
					ptr[j + i * bufsiz] = rfd[j];
				}
				write(fd1, rfd, sizeof(rfd));

			}
			for(j=0;j<2;j++){
				for(i=0;i<bufsiz;i++)
				{
					write(io0,ptr,sizeof(int));
					execlp("io","io","0","256",(char*)NULL);
				}	
				for(i=0;i<bufsiz;i++)
				{
					write(io1,ptr,sizeof(int));
					execlp("io","io","1","256",(char*)NULL);
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
