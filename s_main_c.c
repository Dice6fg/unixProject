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
	int status;
	
	mkfifo("datap0io0", 0644 | O_TRUNC);	
	mkfifo("cmdio0p0", 0644 | O_TRUNC);
	mkfifo("datap0io1", 0644 | O_TRUNC);
	mkfifo("cmdio1p0", 0644 | O_TRUNC);

	mkfifo("datap1io0", 0644 | O_TRUNC);
	mkfifo("cmdio0p1", 0644 | O_TRUNC);
	mkfifo("datap1io1", 0644 | O_TRUNC);
	mkfifo("cmdio1p1", 0644 | O_TRUNC);

	mkfifo("datap2io0", 0644 | O_TRUNC);	
	mkfifo("cmdio0p2", 0644 | O_TRUNC);
	mkfifo("datap2io1", 0644 | O_TRUNC);
	mkfifo("cmdio1p2", 0644 | O_TRUNC);

	mkfifo("datap3io0", 0644 | O_TRUNC);
	mkfifo("cmdio0p3", 0644 | O_TRUNC);
	mkfifo("datap3io1", 0644 | O_TRUNC);
	mkfifo("cmdio1p3", 0644 | O_TRUNC);

	switch(pid = fork())
	{
	case 0:
		execlp("./s_p_c", "s_p_c", "0", "64", (char*)NULL);
		exit(0);
	default: 
		break;
	}

	switch(pid = fork())
	{
	case 0:
		execlp("./s_p_c", "s_p_c", "1", "64", (char*)NULL);
		exit(0);
	default: 
		break;
	}
	
	switch(pid = fork())
	{
	case 0:
		execlp("./s_p_c", "s_p_c", "2", "64", (char*)NULL);
		exit(0);
	default: 
		break;
	}

	switch(pid = fork())
	{
	case 0:
		execlp("./s_p_c", "s_p_c", "3", "64", (char*)NULL);
		exit(0);
	default: 
		break;
	}

	switch(pid = fork())
	{
	case 0:
		execlp("./s_io_c", "s_io_c", "0", "64", (char*)NULL);
		exit(0);
	default: 
		break;
	}

	switch(pid = fork())
	{
	case 0:
		execlp("./s_io_c", "s_io_c", "1", "64", (char*)NULL);
		exit(0);
	default: 
		break;
	}

	while(wait(&status) != -1)
	{
		continue;
	}
	printf("finish 64\n");
	
	switch(pid = fork())
	{
	case 0:
		execlp("./s_p_c", "s_p_c", "0", "256", (char*)NULL);
		exit(0);
	default: 
		break;
	}

	switch(pid = fork())
	{
	case 0:
		execlp("./s_p_c", "s_p_c", "1", "256", (char*)NULL);
		exit(0);
	default: 
		break;
	}
	
	switch(pid = fork())
	{
	case 0:
		execlp("./s_p_c", "s_p_c", "2", "256", (char*)NULL);
		exit(0);
	default: 
		break;
	}

	switch(pid = fork())
	{
	case 0:
		execlp("./s_p_c", "s_p_c", "3", "256", (char*)NULL);
		exit(0);
	default: 
		break;
	}

	switch(pid = fork())
	{
	case 0:
		execlp("./s_io_c", "s_io_c", "0", "256", (char*)NULL);
		exit(0);
	default: 
		break;
	}

	switch(pid = fork())
	{
	case 0:
		execlp("./s_io_c", "s_io_c", "1", "256", (char*)NULL);
		exit(0);
	default: 
		break;
	}
	
	while(wait(&status) != -1)
	{
		continue;
	}
	printf("finish 256\n");

	return 0;
}