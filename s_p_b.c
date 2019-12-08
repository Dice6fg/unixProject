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
	int rfd2[2000];
	int ptr1[4][1000];
	int ptr2[16][1000];
	int io0, io1;
	int cmd0, cmd1;
	int cmdbuf0[64], cmdbuf1[64];
	char* fifoname;
	int bufsiz;
	int b1 = 0, b2 = 0;
	int w = 0;
	int t1, t2;
	
	t1 = 0;
	t2 = 0;
	
	if (atoi(argv[2]) == 64) 
	{
		bufsiz = 1000;	
		fd = open("data1.txt", O_RDONLY);
		switch (atoi(argv[1]))
		{
			case 0: 
				fd1 = open("computenode1_64.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
				cmd0 = open("cmdio0p0", O_RDWR | O_NONBLOCK);
				cmd1 = open("cmdio1p0", O_RDWR | O_NONBLOCK);
				io0 = open("datap0io0", O_RDWR);
				io1 = open("datap0io1", O_RDWR);
								
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
				
				//read cmd
				while(1)
				{
					b1 = read(cmd0, cmdbuf0, sizeof(int)*16);
					if(b1 > 0)
						break;
					lseek(cmd0, 0, SEEK_SET);
				}
				printf("p0 cmd0 read success\n");
				while(1)
				{
					b2 = read(cmd1, cmdbuf1, sizeof(int)*16);
					if(b2 > 0)
						break;
					lseek(cmd1, 0, SEEK_SET);
				}
				printf("p0 cmd1 read success\n");

				//send data
				while(1)
				{
					if(ptr1[t1][0] >= cmdbuf0[t2] && ptr1[t1][999] < cmdbuf0[t2+1]){
						write(io0, ptr1[t1], sizeof(int)*1000);
					//	printf("p0 send data to io0 %d\n", ptr1[t1][0]);
					}
					if(ptr1[t1][0] >= cmdbuf1[t2] && ptr1[t1][999] < cmdbuf1[t2+1]){
						write(io1, ptr1[t2], sizeof(int)*1000);
						printf("p0 send data to io1 %d\n", ptr1[t1][0]);
					}

					t2+=2;
					if(t2 >= 16)
					{
						t2 = 0; t1++;
					}
					if(t1 >= 4)
						break;
				}
		
				printf("p0 finished\n");
				close(fd1);
				close(cmd0);
				close(cmd1);
				close(io0);
				close(io1);
				break;
			case 1: 
				fd1 = open("computenode2_64.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
				cmd0 = open("cmdio0p1", O_RDONLY | O_NONBLOCK);
				cmd1 = open("cmdio1p1", O_RDONLY | O_NONBLOCK);
				io0 = open("datap1io0", O_WRONLY);
				io1 = open("datap1io1", O_WRONLY);
								
				for (i = 0; i < 4; i++)
				{
					lseek(fd, sizeof(int)*bufsiz*(2*i + 1), SEEK_SET);

					read(fd, rfd1, sizeof(rfd1));

					for (j = 0; j < bufsiz; j++)
					{
						ptr1[i][j] = rfd1[j];
					}

					write(fd1, rfd1, sizeof(rfd1));
				}
				
				//read cmd
				while(1)
				{
					b1 = read(cmd0, cmdbuf0, sizeof(int)*16);
					if(b1 > 0)
						break;
					lseek(cmd0, 0, SEEK_SET);
				}
				printf("p1 cmd0 read success\n");
				while(1)
				{
					b2 = read(cmd1, cmdbuf1, sizeof(int)*16);
					if(b2 > 0)
						break;
					lseek(cmd1, 0, SEEK_SET);
				}
				printf("p1 cmd1 read success\n");

				//send data
				while(1)
				{
					if(ptr1[t1][0] >= cmdbuf0[t2] && ptr1[t1][999] < cmdbuf0[t2+1]){
						write(io0, ptr1[t1], sizeof(int)*1000);
					//	printf("p1 send data to io0 %d\n", ptr1[t1][0]);
					}
					if(ptr1[t1][0] >= cmdbuf1[t2] && ptr1[t1][999] < cmdbuf1[t2+1]){
						printf("p1 send data to io1 %d\n", ptr1[t1][0]);
						write(io1, ptr1[t1], sizeof(int)*1000);
					}

					t2+=2;
					if(t2 >= 16)
					{
						t2 = 0; t1++;
					}
					if(t1 >= 4)
						break;
				}
		
				printf("p1 finished\n");
	
				close(fd1);
				close(cmd0);
				close(cmd1);
				close(io0);
				close(io1);
				break;
			case 2: 
				fd1 = open("computenode3_64.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
				cmd0 = open("cmdio0p2", O_RDONLY | O_NONBLOCK);
				cmd1 = open("cmdio1p2", O_RDONLY | O_NONBLOCK);
				io0 = open("datap2io0", O_WRONLY);
				io1 = open("datap2io1", O_WRONLY);
								
				for (i = 0; i < 4; i++)
				{
					lseek(fd, sizeof(int)*bufsiz*(2*i + 8), SEEK_SET);

					read(fd, rfd1, sizeof(rfd1));

					for (j = 0; j < bufsiz; j++)
					{
						ptr1[i][j] = rfd1[j];
					}

					write(fd1, rfd1, sizeof(rfd1));
				}
				
				//read cmd
				while(1)
				{
					b1 = read(cmd0, cmdbuf0, sizeof(int)*16);
					if(b1 > 0)
						break;
					lseek(cmd0, 0, SEEK_SET);
				}
				printf("p2 cmd0 read success\n");
				while(1)
				{
					b2 = read(cmd1, cmdbuf1, sizeof(int)*16);
					if(b2 > 0)
						break;
					lseek(cmd1, 0, SEEK_SET);
				}
				printf("p2 cmd1 read success\n");

				//send data
				while(1)
				{
					if(ptr1[t1][0] >= cmdbuf0[t2] && ptr1[t1][999] < cmdbuf0[t2+1]){
						write(io0, ptr1[t1], sizeof(int)*1000);
					//	printf("p2 send data to io0 %d\n", ptr1[t1][0]);
					}
					if(ptr1[t1][0] >= cmdbuf1[t2] && ptr1[t1][999] < cmdbuf1[t2+1]){
						write(io1, ptr1[t1], sizeof(int)*1000);
						printf("p2 send data to io1 %d\n", ptr1[t1][0]);
					}

					t2+=2;
					if(t2 >= 16)
					{
						t2 = 0; t1++;
					}
					if(t1 >= 4)
						break;
				}
		
				printf("p2 finished\n");
				close(cmd0);
				close(cmd1);
				close(io0);
				close(io1);
				break;

			case 3:
				fd1 = open("computenode4_64.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
				cmd0 = open("cmdio0p3", O_RDONLY | O_NONBLOCK);
				cmd1 = open("cmdio1p3", O_RDONLY | O_NONBLOCK);
				io0 = open("datap3io0", O_WRONLY);
				io1 = open("datap3io1", O_WRONLY);
								
				for (i = 0; i < 4; i++)
				{
					lseek(fd, sizeof(int)*bufsiz*(2*i + 9), SEEK_SET);

					read(fd, rfd1, sizeof(rfd1));

					for (j = 0; j < bufsiz; j++)
					{
						ptr1[i][j] = rfd1[j];
					}

					write(fd1, rfd1, sizeof(rfd1));
				}
				
				//read cmd
				while(1)
				{
					b1 = read(cmd0, cmdbuf0, sizeof(int)*16);
					if(b1 > 0)
						break;
					lseek(cmd0, 0, SEEK_SET);
				}
				printf("p3 cmd0 read success\n");
				while(1)
				{
					b2 = read(cmd1, cmdbuf1, sizeof(int)*16);
					if(b2 > 0)
						break;
					lseek(cmd1, 0, SEEK_SET);
				}
				printf("p3 cmd1 read success\n");

				//send data
				while(1)
				{
					if(ptr1[t1][0] >= cmdbuf0[t2] && ptr1[t1][999] < cmdbuf0[t2+1]){
						write(io0, ptr1[t1], sizeof(int)*1000);
					//	printf("p3 send data to io0 %d\n", ptr1[t1][0]);
					}
					if(ptr1[t1][0] >= cmdbuf1[t2] && ptr1[t1][999] < cmdbuf1[t2+1]){
						write(io1, ptr1[t1], sizeof(int)*1000);
						printf("p3 send data to io1 %d\n", ptr1[t1][0]);
					}

					t2+=2;
					if(t2 >= 16)
					{
						t2 = 0; t1++;
					}
					if(t1 >= 4)
						break;
				}
		
				printf("p3 finished\n");
	
				close(fd1);
				close(cmd0);
				close(cmd1);
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
				fd1 = open("computenode1_256.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
				cmd0 = open("cmdio0p0", O_RDWR | O_NONBLOCK);
				cmd1 = open("cmdio1p0", O_RDWR | O_NONBLOCK);
				io0 = open("datap0io0", O_RDWR);
				io1 = open("datap0io1", O_RDWR);
								
				for (i = 0; i < 16; i++)
				{
					lseek(fd, sizeof(int)*bufsiz*2*i, SEEK_SET);

					read(fd, rfd1, sizeof(rfd1));

					for (j = 0; j < bufsiz; j++)
					{
						ptr2[i][j] = rfd1[j];
					}

					write(fd1, rfd1, sizeof(rfd1));
				}
				
				//read cmd
				while(1)
				{
					b1 = read(cmd0, cmdbuf0, sizeof(int)*64);
					if(b1 > 0)
						break;
					lseek(cmd0, 0, SEEK_SET);
				}
				printf("p0 cmd0 read success\n");
				while(1)
				{
					b2 = read(cmd1, cmdbuf1, sizeof(int)*64);
					if(b2 > 0)
						break;
					lseek(cmd1, 0, SEEK_SET);
				}
				printf("p0 cmd1 read success\n");

				//send data
				while(1)
				{
					if(ptr2[t1][0] >= cmdbuf0[t2] && ptr2[t1][999] < cmdbuf0[t2+1]){
						write(io0, ptr2[t1], sizeof(int)*1000);
					//	printf("p0 send data to io0 %d\n", ptr2[t1][0]);
					}
					if(ptr2[t1][0] >= cmdbuf1[t2] && ptr2[t1][999] < cmdbuf1[t2+1]){
						write(io1, ptr2[t1], sizeof(int)*1000);
						printf("p0 send data to io1 %d\n", ptr2[t1][0]);
					}

					t2+=2;
					if(t2 >= 64)
					{
						t2 = 0; t1++;
					}
					if(t1 >= 16)
						break;
				}
		
				printf("p0 finished\n");
				close(fd1);
				close(cmd0);
				close(cmd1);
				close(io0);
				close(io1);
				break;
			case 1:
				fd1 = open("computenode2_256.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
				cmd0 = open("cmdio0p1", O_RDWR | O_NONBLOCK);
				cmd1 = open("cmdio1p1", O_RDWR | O_NONBLOCK);
				io0 = open("datap1io0", O_RDWR);
				io1 = open("datap1io1", O_RDWR);
								
				for (i = 0; i < 16; i++)
				{
					lseek(fd, sizeof(int)*bufsiz*(2*i + 1), SEEK_SET);

					read(fd, rfd1, sizeof(rfd1));

					for (j = 0; j < bufsiz; j++)
					{
						ptr2[i][j] = rfd1[j];
					}

					write(fd1, rfd1, sizeof(rfd1));
				}
				
				//read cmd
				while(1)
				{
					b1 = read(cmd0, cmdbuf0, sizeof(int)*64);
					if(b1 > 0)
						break;
					lseek(cmd0, 0, SEEK_SET);
				}
				printf("p0 cmd0 read success\n");
				while(1)
				{
					b2 = read(cmd1, cmdbuf1, sizeof(int)*64);
					if(b2 > 0)
						break;
					lseek(cmd1, 0, SEEK_SET);
				}
				printf("p1 cmd1 read success\n");

				//send data
				while(1)
				{
					if(ptr2[t1][0] >= cmdbuf0[t2] && ptr2[t1][999] < cmdbuf0[t2+1]){
						write(io0, ptr2[t1], sizeof(int)*1000);
					//	printf("p1 send data to io0 %d\n", ptr2[t1][0]);
					}
					if(ptr2[t1][0] >= cmdbuf1[t2] && ptr2[t1][999] < cmdbuf1[t2+1]){
						write(io1, ptr2[t1], sizeof(int)*1000);
						printf("p1 send data to io1 %d\n", ptr2[t1][0]);
					}

					t2+=2;
					if(t2 >= 64)
					{
						t2 = 0; t1++;
					}
					if(t1 >= 16)
						break;
				}
		
				printf("p1 finished\n");
				close(fd1);
				close(cmd0);
				close(cmd1);
				close(io0);
				close(io1);
				break;
			case 2:
				fd1 = open("computenode3_256.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
				cmd0 = open("cmdio0p2", O_RDWR | O_NONBLOCK);
				cmd1 = open("cmdio1p2", O_RDWR | O_NONBLOCK);
				io0 = open("datap2io0", O_RDWR);
				io1 = open("datap2io1", O_RDWR);
								
				for (i = 0; i < 16; i++)
				{
					lseek(fd, sizeof(int)*bufsiz*(2*i + 32), SEEK_SET);

					read(fd, rfd1, sizeof(rfd1));

					for (j = 0; j < bufsiz; j++)
					{
						ptr2[i][j] = rfd1[j];
					}

					write(fd1, rfd1, sizeof(rfd1));
				}
				
				//read cmd
				while(1)
				{
					b1 = read(cmd0, cmdbuf0, sizeof(int)*64);
					if(b1 > 0)
						break;
					lseek(cmd0, 0, SEEK_SET);
				}
				printf("p2 cmd0 read success\n");
				while(1)
				{
					b2 = read(cmd1, cmdbuf1, sizeof(int)*64);
					if(b2 > 0)
						break;
					lseek(cmd1, 0, SEEK_SET);
				}
				printf("p2 cmd1 read success\n");

				//send data
				while(1)
				{
					if(ptr2[t1][0] >= cmdbuf0[t2] && ptr2[t1][999] < cmdbuf0[t2+1]){
						write(io0, ptr2[t1], sizeof(int)*1000);
					//	printf("p0 send data to io0 %d\n", ptr2[t1][0]);
					}
					if(ptr2[t1][0] >= cmdbuf1[t2] && ptr2[t1][999] < cmdbuf1[t2+1]){
						write(io1, ptr2[t1], sizeof(int)*1000);
						printf("p2 send data to io1 %d\n", ptr2[t1][0]);
					}

					t2+=2;
					if(t2 >= 64)
					{
						t2 = 0; t1++;
					}
					if(t1 >= 16)
						break;
				}
		
				printf("p2 finished\n");
				close(fd1);
				close(cmd0);
				close(cmd1);
				close(io0);
				close(io1);
				break;
			case 3:
				fd1 = open("computenode4_256.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
				cmd0 = open("cmdio0p3", O_RDWR | O_NONBLOCK);
				cmd1 = open("cmdio1p3", O_RDWR | O_NONBLOCK);
				io0 = open("datap3io0", O_RDWR);
				io1 = open("datap3io1", O_RDWR);
								
				for (i = 0; i < 16; i++)
				{
					lseek(fd, sizeof(int)*bufsiz*(2*i + 33), SEEK_SET);

					read(fd, rfd1, sizeof(rfd1));

					for (j = 0; j < bufsiz; j++)
					{
						ptr2[i][j] = rfd1[j];
					}

					write(fd1, rfd1, sizeof(rfd1));
				}
				
				//read cmd
				while(1)
				{
					b1 = read(cmd0, cmdbuf0, sizeof(int)*64);
					if(b1 > 0)
						break;
					lseek(cmd0, 0, SEEK_SET);
				}
				printf("p3 cmd0 read success\n");
				while(1)
				{
					b2 = read(cmd1, cmdbuf1, sizeof(int)*64);
					if(b2 > 0)
						break;
					lseek(cmd1, 0, SEEK_SET);
				}
				printf("p3 cmd1 read success\n");

				//send data
				while(1)
				{
					if(ptr2[t1][0] >= cmdbuf0[t2] && ptr2[t1][999] < cmdbuf0[t2+1]){
						write(io0, ptr2[t1], sizeof(int)*1000);
					//	printf("p3 send data to io0 %d\n", ptr2[t1][0]);
					}
					if(ptr2[t1][0] >= cmdbuf1[t2] && ptr2[t1][999] < cmdbuf1[t2+1]){
						write(io1, ptr2[t1], sizeof(int)*1000);
						printf("p3 send data to io1 %d\n", ptr2[t1][0]);
					}

					t2+=2;
					if(t2 >= 64)
					{
						t2 = 0; t1++;
					}
					if(t1 >= 16)
						break;
				}
		
				printf("p3 finished\n");
				close(fd1);
				close(cmd0);
				close(cmd1);
				close(io0);
				close(io1);
				break;
			default: break;
		}
		close(fd);
	}
}
