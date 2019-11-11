#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<malloc.h>

int main(int argc, char *argv[], char **envp)
{
	
	int fd,fd1,fd2,fd3;
	int *ptr;
	int i;
	int* rfd
	int* wfd

	int io0, io1;
	char* fifoname;



	for(i=0; i<sizeof(int)*4000; i++)
	{
		write(io0, ptr, sizof(int));
	}

	close(io0);
	close(io1);

	if(argv[2] == 64 && argv[1] == 0) 
	{
		io0 = open("fifo0", O_WRONLY);
		io1 = open("fifo4", O_WRONLY);

		rfd = (int*)malloc(sizeof(int)*1000);

		fd = open("data1.txt",O_RDONLY);

		fd1 = open("computenode1_64.txt",O_CREAT | O_RDWR | O_TRUNC , 0644);

		ptr = malloc(sizeof(int) * 4000);

		read(fd,rfd,sizeof(rfd));

		for(i=0;i<1000;i++)
			ptr[i] = rfd[i];

		write(fd1,rfd,sizeof(rfd));

		lseek(fd,sizeof(int)*1000,SEEK_CUR);


		read(fd,rfd,sizeof(rfd));

		for(i=0;i<1000;i++)
	  		ptr[i+1000] = rfd[i];

		write(fd1,rfd,sizeof(rfd));
		lseek(fd,sizeof(int)*1000,SEEK_CUR);

	  	read(fd,rfd,sizeof(rfd));
	 	for(i=0;i<1000;i++)
          	ptr[i+2000] = rfd[i];
	 	write(fd1,rfd,sizeof(rfd));
	 	lseek(fd,sizeof(int)*1000,SEEK_CUR);
	 	read(fd,rfd,sizeof(rfd));
	  	for(i=0;i<1000;i++)
	  	ptr[i+3000] = rfd[i];
	 	write(fd1,rfd,sizeof(rfd));
		lseek(fd,0,SEEK_SET);

		close(io0);
		close(io1);
		
	}
	
	else if(argv[2] == 64 && argv[1] == 1) 
	{
		mkfifo("fifo1", 0644);
		mkfifo("fifo5", 0644);
		rfd = (int*)malloc(sizeof(int)*1000);
		fd = open("data1.txt",O_RDONLY);
		fd2 = open("computenode2.txt_64",O_CREAT | O_RDWR | O_TRUNC , 0644);
		ptr = malloc(sizeof(int) * 4000);
		lseek(fd,sizeof(int)*1000,SEEK_CUR); 
		read(fd,rfd,sizeof(rfd));
		for(i=0;i<1000;i++)
		ptr[i] = rfd[i];
		write(fd2,rfd,sizeof(rfd));
		lseek(fd,sizeof(int)*1000,SEEK_CUR); 
		read(fd,rfd,sizeof(rfd));
		for(i=0;i<1000;i++)
	  	ptr[i+1000] = rfd[i];
		write(fd2,rfd,sizeof(rfd));
		lseek(fd,sizeof(int)*1000,SEEK_CUR);
	  	read(fd,rfd,sizeof(rfd));
	 	for(i=0;i<1000;i++)
          	ptr[i+2000] = rfd[i];
	 	write(fd2,rfd,sizeof(rfd));
	 	lseek(fd,sizeof(int)*1000,SEEK_CUR);
	 	read(fd,rfd,sizeof(rfd));
	  	for(i=0;i<1000;i++)
	  	ptr[i+3000] = rfd[i];
	 	write(fd2,rfd,sizeof(rfd));
		lseek(fd,0,SEEK_SET);
	}
	else if(argv[2] == 64 && argv[1] == 2) 
	{
		mkfifo("fifo2", 0644);
		mkfifo("fifo6", 0644);
		rfd = (int*)malloc(sizeof(int)*1000);
		fd = open("data1.txt",O_RDONLY);
		fd1 = open("computenode3.txt_64",O_CREAT | O_RDWR | O_TRUNC , 0644);
		ptr = malloc(sizeof(int) * 4000);
	 	lseek(fd,sizeof(int)*8000,SEEK_CUR);
		read(fd,rfd,sizeof(rfd));
		for(i=0;i<1000;i++)
		ptr[i] = rfd[i];
		write(fd1,rfd,sizeof(rfd));
		lseek(fd,sizeof(int)*1000,SEEK_CUR); 
		read(fd,rfd,sizeof(rfd));
		for(i=0;i<1000;i++)
	  	ptr[i+1000] = rfd[i];
		write(fd1,rfd,sizeof(rfd));
		lseek(fd,sizeof(int)*1000,SEEK_CUR);
	  	read(fd,rfd,sizeof(rfd));
	 	for(i=0;i<1000;i++)
          	ptr[i+2000] = rfd[i];
	 	write(fd1,rfd,sizeof(rfd));
	 	lseek(fd,sizeof(int)*1000,SEEK_CUR);
	 	read(fd,rfd,sizeof(rfd));
	  	for(i=0;i<1000;i++)
	  	ptr[i+3000] = rfd[i];
	 	write(fd1,rfd,sizeof(rfd));
		lseek(fd,0,SEEK_SET);
	}
	else if(argv[2] == 64 && argv[1] == 3) 
	{
		mkfifo("fifo3", 0644);
		mkfifo("fifo7", 0644);
		rfd = (int*)malloc(sizeof(int)*1000);
		fd = open("data1.txt",O_RDONLY);
		fd1 = open("computenode4.txt_64",O_CREAT | O_RDWR | O_TRUNC , 0644);
		ptr = malloc(sizeof(int) * 4000);
	 	lseek(fd,sizeof(int)*9000,SEEK_CUR);
		read(fd,rfd,sizeof(rfd));
		for(i=0;i<1000;i++)
		ptr[i] = rfd[i];
		write(fd1,rfd,sizeof(rfd));
		lseek(fd,sizeof(int)*1000,SEEK_CUR); 
		read(fd,rfd,sizeof(rfd));
		for(i=0;i<1000;i++)
	  	ptr[i+1000] = rfd[i];
		write(fd1,rfd,sizeof(rfd));
		lseek(fd,sizeof(int)*1000,SEEK_CUR);
	  	read(fd,rfd,sizeof(rfd));
	 	for(i=0;i<1000;i++)
          	ptr[i+2000] = rfd[i];
	 	write(fd1,rfd,sizeof(rfd));
	 	lseek(fd,sizeof(int)*1000,SEEK_CUR);
	 	read(fd,rfd,sizeof(rfd));
	  	for(i=0;i<1000;i++)
	  	ptr[i+3000] = rfd[i];
	 	write(fd1,rfd,sizeof(rfd));
		lseek(fd,0,SEEK_SET);
	}

	else if(argv[2] == 256 && argv[1] == 0)
        {
		mkfifo("fifo0", 0644);
		mkfifo("fifo4", 0644);
		rfd = (int*)malloc(sizeof(int)*4000);
                fd = open("data2.txt",O_RDONLY);
                fd1 = open("computenode1.txt_256",O_CREAT | O_RDWR | O_TRUNC , 0644);
                ptr = malloc(sizeof(int) * 16000);
	    	lseek(fd,0,SEEK_SET);
                read(fd,rfd,sizeof(rfd));
                for(i=0;i<4000;i++)
                ptr[i] = rfd[i];
                write(fd1,rfd,sizeof(rfd));
                lseek(fd,sizeof(int)*4000,SEEK_CUR);
                read(fd,rfd,sizeof(rfd));
                for(i=0;i<4000;i++)
                ptr[i+4000] = rfd[i];
                write(fd1,rfd,sizeof(rfd));
                lseek(fd,sizeof(int)*4000,SEEK_CUR);
                read(fd,rfd,sizeof(rfd));
                for(i=0;i<4000;i++)
                ptr[i+8000] = rfd[i];
                write(fd1,rfd,sizeof(rfd));
                lseek(fd,sizeof(int)*4000,SEEK_CUR);
                read(fd,rfd,sizeof(rfd));
                for(i=0;i<4000;i++)
                ptr[i+12000] = rfd[i];
                write(fd1,rfd,sizeof(rfd));
                lseek(fd,0,SEEK_SET);
        }
	else if(argv[2] == 256 && argv[1] == 1)
        {
		mkfifo("fifo0", 0644);
		mkfifo("fifo4", 0644);
		rfd = (int*)malloc(sizeof(int)*4000);
                fd = open("data2.txt",O_RDONLY);
                fd1 = open("computenode2.txt_256",O_CREAT | O_RDWR | O_TRUNC , 0644);
                ptr = malloc(sizeof(int) * 16000);
	    	lseek(fd,4000,SEEK_SET);
                read(fd,rfd,sizeof(rfd));
                for(i=0;i<4000;i++)
                ptr[i] = rfd[i];
                write(fd1,rfd,sizeof(rfd));
                lseek(fd,sizeof(int)*4000,SEEK_CUR);
                read(fd,rfd,sizeof(rfd));
                for(i=0;i<4000;i++)
                ptr[i+4000] = rfd[i];
                write(fd1,rfd,sizeof(rfd));
                lseek(fd,sizeof(int)*4000,SEEK_CUR);
                read(fd,rfd,sizeof(rfd));
                for(i=0;i<4000;i++)
                ptr[i+8000] = rfd[i];
                write(fd1,rfd,sizeof(rfd));
                lseek(fd,sizeof(int)*4000,SEEK_CUR);
                read(fd,rfd,sizeof(rfd));
                for(i=0;i<4000;i++)
                ptr[i+12000] = rfd[i];
                write(fd1,rfd,sizeof(rfd));
                lseek(fd,0,SEEK_SET);
        }
	else if(argv[2] == 256 && argv[1] == 2)
        {
		mkfifo("fifo0", 0644);
		mkfifo("fifo4", 0644);
		rfd = (int*)malloc(sizeof(int)*4000);
                fd = open("data2.txt",O_RDONLY);
                fd1 = open("computenode3.txt_256",O_CREAT | O_RDWR | O_TRUNC , 0644);
                ptr = malloc(sizeof(int) * 16000);
	    	lseek(fd,32000,SEEK_SET);
                read(fd,rfd,sizeof(rfd));
                for(i=0;i<4000;i++)
                ptr[i] = rfd[i];
                write(fd1,rfd,sizeof(rfd));
                lseek(fd,sizeof(int)*4000,SEEK_CUR);
                read(fd,rfd,sizeof(rfd));
                for(i=0;i<4000;i++)
                ptr[i+4000] = rfd[i];
                write(fd1,rfd,sizeof(rfd));
                lseek(fd,sizeof(int)*4000,SEEK_CUR);
                read(fd,rfd,sizeof(rfd));
                for(i=0;i<4000;i++)
                ptr[i+8000] = rfd[i];
                write(fd1,rfd,sizeof(rfd));
                lseek(fd,sizeof(int)*4000,SEEK_CUR);
                read(fd,rfd,sizeof(rfd));
                for(i=0;i<4000;i++)
                ptr[i+12000] = rfd[i];
                write(fd1,rfd,sizeof(rfd));
                lseek(fd,0,SEEK_SET);
        }
	else if(argv[2] == 256 && argv[1] == 3)
        {
		mkfifo("fifo0", 0644);
		mkfifo("fifo4", 0644);
		rfd = (int*)malloc(sizeof(int)*4000);
                fd = open("data2.txt",O_RDONLY);
                fd1 = open("computenode4.txt_256",O_CREAT | O_RDWR | O_TRUNC , 0644);
                ptr = malloc(sizeof(int) * 16000);
	   	lseek(fd,36000,SEEK_SET);
                read(fd,rfd,sizeof(rfd));
                for(i=0;i<4000;i++)
                ptr[i] = rfd[i];
                write(fd1,rfd,sizeof(rfd));
                lseek(fd,sizeof(int)*4000,SEEK_CUR);
                read(fd,rfd,sizeof(rfd));
                for(i=0;i<4000;i++)
                ptr[i+4000] = rfd[i];
                write(fd1,rfd,sizeof(rfd));
                lseek(fd,sizeof(int)*4000,SEEK_CUR);
                read(fd,rfd,sizeof(rfd));
                for(i=0;i<4000;i++)
                ptr[i+8000] = rfd[i];
                write(fd1,rfd,sizeof(rfd));
                lseek(fd,sizeof(int)*4000,SEEK_CUR);
                read(fd,rfd,sizeof(rfd));
                for(i=0;i<4000;i++)
                ptr[i+12000] = rfd[i];
                write(fd1,rfd,sizeof(rfd));
                lseek(fd,0,SEEK_SET);
        }
		

}
