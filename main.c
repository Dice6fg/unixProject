#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main(void)
{
	int pid;
	int fd;
	
#ifdef TIMES
	start = dclock();
#endif

	switch(pid = fork())
	{
	case 0:
		execlp("p", "p", "0", "64", (char*)NULL);
		exit(0);
	default: 
		break;
	}

	switch(pid = fork())
	{
	case 0:
		execlp("p", "p", "1", "64", (char*)NULL);
		exit(0);
	default: 
		break;
	}
	
	switch(pid = fork())
	{
	case 0:
		execlp("p", "p", "2", "64", (char*)NULL);
		exit(0);
	default: 
		break;
	}

	switch(pid = fork())
	{
	case 0:
		execlp("p", "p", "3", "64", (char*)NULL);
		exit(0);
	default: 
		break;
	}

	switch(pid = fork())
	{
	case 0:
		execlp("io", "io", "0", "64", (char*)NULL);
		exit(0);
	default: 
		break;
	}

	switch(pid = fork())
	{
	case 0:
		execlp("io", "io", "1", "64", (char*)NULL);
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
