#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(void)
{
	int pid;
	int fd;
	
#ifdef TIMES
	start = dclock();
#endif
	
	mkfifo("fifo0", 0644);	
	mkfifo("fifo1", 0644);
	mkfifo("fifo2", 0644);
	mkfifo("fifo3", 0644);
	mkfifo("fifo4", 0644);
	mkfifo("fifo5", 0644);
	mkfifo("fifo6", 0644);
	mkfifo("fifo7", 0644);

	switch(pid = fork())
	{
	case 0:
		execlp("./p", "p", "0", "64", (char*)NULL);
		exit(0);
	default: 
		break;
	}

	switch(pid = fork())
	{
	case 0:
		execlp("./p", "p", "1", "64", (char*)NULL);
		exit(0);
	default: 
		break;
	}
	
	switch(pid = fork())
	{
	case 0:
		execlp("./p", "p", "2", "64", (char*)NULL);
		exit(0);
	default: 
		break;
	}

	switch(pid = fork())
	{
	case 0:
		execlp("./p", "p", "3", "64", (char*)NULL);
		exit(0);
	default: 
		break;
	}

	switch(pid = fork())
	{
	case 0:
		execlp("./io", "io", "0", "64", (char*)NULL);
		exit(0);
	default: 
		break;
	}

	switch(pid = fork())
	{
	case 0:
		execlp("./io", "io", "1", "64", (char*)NULL);
		exit(0);
	default: 
		break;
	}
	
	wait(NULL);	

#ifdef TIMES
	end = dclock()-start;
	printf(end);
#endif

	return 0;
}
