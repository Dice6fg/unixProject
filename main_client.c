#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


//client-orient에 필요한 파이프 만들고 p_client 실행
int main(void)
{
	int pid;
	int fd;
	int status;
	
    //compute-> io 파이프
	mkfifo("fifo0", 0644);	
	mkfifo("fifo1", 0644);
	mkfifo("fifo2", 0644);
	mkfifo("fifo3", 0644);
	mkfifo("fifo4", 0644);
	mkfifo("fifo5", 0644);
	mkfifo("fifo6", 0644);
	mkfifo("fifo7", 0644);

    //client-oriented 데이터 전송 파이프 
    mkfifo("1to2", 0644);
    mkfifo("1to3", 0644);
    mkfifo("1to4", 0644);
    mkfifo("2to1", 0644);
	mkfifo("2to3", 0644);
    mkfifo("2to4", 0644);
    mkfifo("3to1", 0644);
    mkfifo("3to2", 0644);
	mkfifo("3to4", 0644);
    mkfifo("4to1", 0644);
    mkfifo("4to2", 0644);
    mkfifo("4to3", 0644);

	//client-oriented 신호 전송 파이프 
	mkfifo("1to2_s", 0644);
    mkfifo("1to3_s", 0644);
    mkfifo("1to4_s", 0644);
    mkfifo("2to1_s", 0644);
	mkfifo("2to3_s", 0644);
    mkfifo("2to4_s", 0644);
    mkfifo("3to1_s", 0644);
    mkfifo("3to2_s", 0644);
	mkfifo("3to4_s", 0644);
    mkfifo("4to1_s", 0644);
    mkfifo("4to2_s", 0644);
	printf("asd");
    mkfifo("4to3_s", 0644);

	printf("asd");
	printf("asdasfasv");
	switch(pid = fork())
	{
	case 0:
		execlp("./p_client", "p_client", "0", "64", (char*)NULL);
		exit(0);
	default: 
		break;
	}

	switch(pid = fork())
	{
	case 0:
		execlp("./p_client", "p_client", "1", "64", (char*)NULL);
		exit(0);
	default: 
		break;
	}
	
	switch(pid = fork())
	{
	case 0:
		execlp("./p_client", "p_client", "2", "64", (char*)NULL);
		exit(0);
	default: 
		break;
	}

	switch(pid = fork())
	{
	case 0:
		execlp("./p_client", "p_client", "3", "64", (char*)NULL);
		exit(0);
	default: 
		break;
	}

	switch(pid = fork())
	{
	case 0:
		execlp("./io_client", "io_client", "0", "64", (char*)NULL);
		exit(0);
	default: 
		break;
	}

	switch(pid = fork())
	{
	case 0:
		execlp("./io_client", "io_client", "1", "64", (char*)NULL);
		exit(0);
	default: 
		break;
	}
	
	while(wait(&status) != -1)
	{
		continue;
	}

	
	switch(pid = fork())
	{
	case 0:
		execlp("./p_client", "p_client", "0", "256", (char*)NULL);
		exit(0);
	default: 
		break;
	}

	switch(pid = fork())
	{
	case 0:
		execlp("./p_client", "p_client", "1", "256", (char*)NULL);
		exit(0);
	default: 
		break;
	}
	
	switch(pid = fork())
	{
	case 0:
		execlp("./p_client", "p_client", "2", "256", (char*)NULL);
		exit(0);
	default: 
		break;
	}

	switch(pid = fork())
	{
	case 0:
		execlp("./p_client", "p_client", "3", "256", (char*)NULL);
		exit(0);
	default: 
		break;
	}

	switch(pid = fork())
	{
	case 0:
		execlp("./io_client", "io_client", "0", "256", (char*)NULL);
		exit(0);
	default: 
		break;
	}

	switch(pid = fork())
	{
	case 0:
		execlp("./io_client", "io_client", "1", "256", (char*)NULL);
		exit(0);
	default: 
		break;
	}
	
	while(wait(&status) != -1)
	{
		continue;
	}

	return 0;
}
