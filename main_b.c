#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/time.h>

int main(void)
{
	int pid;
	int fd;
	int status;
	struct timeval start,end;
	double time;

	
	mkfifo("fifo0", 0644);	
	mkfifo("fifo1", 0644);
	mkfifo("fifo2", 0644);
	mkfifo("fifo3", 0644);
	mkfifo("fifo4", 0644);
	mkfifo("fifo5", 0644);
	mkfifo("fifo6", 0644);
	mkfifo("fifo7", 0644);
	
	gettimeofday(&start,NULL);
	
	switch(pid = fork())
	{
	case 0:
		execlp("./p_b", "p_b", "0", "64", (char*)NULL);
		exit(0);
	default: 
		break;
	}

	switch(pid = fork())
	{
	case 0:
		execlp("./p_b", "p_b", "1", "64", (char*)NULL);
		exit(0);
	default: 
		break;
	}
	
	switch(pid = fork())
	{
	case 0:
		execlp("./p_b", "p_b", "2", "64", (char*)NULL);
		exit(0);
	default: 
		break;
	}

	switch(pid = fork())
	{
	case 0:
		execlp("./p_b", "p_b", "3", "64", (char*)NULL);
		exit(0);
	default: 
		break;
	}

	switch(pid = fork())
	{
	case 0:
		execlp("./io_b", "io_b", "0", "64", (char*)NULL);
		exit(0);
	default: 
		break;
	}

	switch(pid = fork())
	{
	case 0:
		execlp("./io_b", "io_b", "1", "64", (char*)NULL);
		exit(0);
	default: 
		break;
	}
	gettimeofday(&end,NULL);
		
	time = ( end.tv_sec - start.tv_sec )*1000000 + (( end.tv_usec - start.tv_usec ));
	
	printf("64 kB time is : %lf\n", time);

	while(wait(&status) != -1)
	{
		continue;
	}

	
	gettimeofday(&start,NULL);
	switch(pid = fork())
	{
	case 0:
		execlp("./p_b", "p_b", "0", "256", (char*)NULL);
		exit(0);
	default: 
		break;
	}

	switch(pid = fork())
	{
	case 0:
		execlp("./p_b", "p_b", "1", "256", (char*)NULL);
		exit(0);
	default: 
		break;
	}
	
	switch(pid = fork())
	{
	case 0:
		execlp("./p_b", "p_b", "2", "256", (char*)NULL);
		exit(0);
	default: 
		break;
	}

	switch(pid = fork())
	{
	case 0:
		execlp("./p_b", "p_b", "3", "256", (char*)NULL);
		exit(0);
	default: 
		break;
	}

	switch(pid = fork())
	{
	case 0:
		execlp("./io_b", "io_b", "0", "256", (char*)NULL);
		exit(0);
	default: 
		break;
	}

	switch(pid = fork())
	{
	case 0:
		execlp("./io_b", "io_b", "1", "256", (char*)NULL);
		exit(0);
	default: 
		break;
	}
	
	gettimeofday(&end,NULL);
		
	time = ( end.tv_sec - start.tv_sec )*1000000 + (( end.tv_usec - start.tv_usec ));
	
	printf("256KB time is : %lf\n", time);

	while(wait(&status) != -1)
	{
		continue;
	}

	return 0;
}
