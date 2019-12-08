#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char* argv[])
{
	int cmd0, cmd1, cmd2, cmd3;
	int data0, data1, data2, data3;	
	int fd;	

	int* buf0, *buf1, *buf2, *buf3, *buf;
	int temp[1000];
	int bufsz;
	int cmdbuf[64];
	
	int count = 0;
	int i;
	int zero = 0;
	int offset = 0;

	int b = 0;
	struct flock lock;	

	if(atoi(argv[2]) == 64)
	{
		bufsz = 8000*sizeof(int);
	}
	else if(atoi(argv[2]) == 256)
	{
		bufsz = 32000*sizeof(int);
	}
	buf = (int*)malloc(bufsz);
	


	if(atoi(argv[2]) == 64)
	{
		if(atoi(argv[1]) == 0)
		{
			cmd0 = open("cmdio0p0", O_WRONLY);
			cmd1 = open("cmdio0p1", O_WRONLY);
			cmd2 = open("cmdio0p2", O_WRONLY);
			cmd3 = open("cmdio0p3", O_WRONLY);

			data0 = open("datap0io0", O_RDONLY | O_NONBLOCK);
			data1 = open("datap1io0", O_RDONLY | O_NONBLOCK);
			data2 = open("datap2io0", O_RDONLY | O_NONBLOCK);
			data3 = open("datap3io0", O_RDONLY | O_NONBLOCK);

			fd = open("io0_64_result.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);

			for(i=0; i<16; i+=2)
			{
				cmdbuf[i] = i*10000;
				cmdbuf[i+1] = cmdbuf[i] + 10000;
			}
		}
		else if(atoi(argv[1]) == 1)
		{
			cmd0 = open("cmdio1p0", O_WRONLY);
			cmd1 = open("cmdio1p1", O_WRONLY);
			cmd2 = open("cmdio1p2", O_WRONLY);
			cmd3 = open("cmdio1p3", O_WRONLY);

			data0 = open("datap0io1", O_RDONLY | O_NONBLOCK);
			data1 = open("datap1io1", O_RDONLY | O_NONBLOCK);
			data2 = open("datap2io1", O_RDONLY | O_NONBLOCK);
			data3 = open("datap3io1", O_RDONLY | O_NONBLOCK);

			fd = open("io1_64_result.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);

			for(i=0; i<16; i+=2)
			{
				cmdbuf[i] = i*10000 + 10000;
				cmdbuf[i+1] = cmdbuf[i] + 10000;
			}
		}
	}
	else if(atoi(argv[2]) == 256)
	{
		if(atoi(argv[1]) == 0)
		{	
			cmd0 = open("cmdio0p0", O_WRONLY);
			cmd1 = open("cmdio0p1", O_WRONLY);
			cmd2 = open("cmdio0p2", O_WRONLY);
			cmd3 = open("cmdio0p3", O_WRONLY);

			data0 = open("datap0io0", O_RDONLY | O_NONBLOCK);
			data1 = open("datap1io0", O_RDONLY | O_NONBLOCK);
			data2 = open("datap2io0", O_RDONLY | O_NONBLOCK);
			data3 = open("datap3io0", O_RDONLY | O_NONBLOCK);

			fd = open("io0_256_result.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);

			for(i=0; i<64; i+=2)
			{
				cmdbuf[i] = i*10000;
				cmdbuf[i+1] = cmdbuf[i] + 10000;
			}
		}
		else if(atoi(argv[1]) == 1)
		{
			cmd0 = open("cmdio1p0", O_WRONLY);
			cmd1 = open("cmdio1p1", O_WRONLY);
			cmd2 = open("cmdio1p2", O_WRONLY);
			cmd3 = open("cmdio1p3", O_WRONLY);

			data0 = open("datap0io1", O_RDONLY | O_NONBLOCK);
			data1 = open("datap1io1", O_RDONLY | O_NONBLOCK);
			data2 = open("datap2io1", O_RDONLY | O_NONBLOCK);
			data3 = open("datap3io1", O_RDONLY | O_NONBLOCK);

			fd = open("io1_256_result.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);

			for(i=0; i<64; i+=2)
			{
				cmdbuf[i] = i*10000 + 10000;
				cmdbuf[i+1] = cmdbuf[i] + 10000;
			}
		}
	}
	
	
	//write cmd
	write(cmd0, cmdbuf, sizeof(int)*atoi(argv[2])/4);
	write(cmd1, cmdbuf, sizeof(int)*atoi(argv[2])/4);
	write(cmd2, cmdbuf, sizeof(int)*atoi(argv[2])/4);
	write(cmd3, cmdbuf, sizeof(int)*atoi(argv[2])/4);
	

	while(1)
	{
		//printf("io%d read data %d have to read: %d\n", atoi(argv[1]), count, atoi(argv[2])/8);

		b = read(data0, temp, sizeof(int)*1000);
		if(b == sizeof(int)*1000)
		{
			b = 0;
			offset = (temp[0]/10 - atoi(argv[1])*1000)/2;	
	if(atoi(argv[1]) == 1)
			printf("io%d data: %d, offset: %d from p0\n", atoi(argv[1]), temp[0], offset);
			if(offset >= 0)
				memcpy(&buf[offset], temp, sizeof(int)*1000);
			count++;
		}
		
		b = read(data1, temp, sizeof(int)*1000);
		if(b == sizeof(int)*1000)
		{
			b = 0;
			offset = (temp[0]/10 - atoi(argv[1])*1000)/2;	
	if(atoi(argv[1]) == 1)
			printf("io%d data: %d, offset: %d from p1\n", atoi(argv[1]), temp[0], offset);
			if(offset >= 0)
			memcpy(&buf[offset], temp, sizeof(int)*1000);
			count++;
		}

		b = read(data2, temp, sizeof(int)*1000);
		if(b == sizeof(int)*1000)
		{
			b = 0;
			offset = (temp[0]/10 - atoi(argv[1])*1000)/2;	
	if(atoi(argv[1]) == 1)
			printf("io%d data: %d, offset: %d from p2\n", atoi(argv[1]), temp[0], offset);
			if(offset >= 0)
			memcpy(&buf[offset], temp, sizeof(int)*1000);
			count++;
		}

		b = read(data3, temp, sizeof(int)*1000);
		if(b == sizeof(int)*1000)
		{
			b = 0;
			offset = (temp[0]/10 - atoi(argv[1])*1000)/2;	
	if(atoi(argv[1]) == 1)
			printf("io%d data: %d, offset: %d from p3\n", atoi(argv[1]), temp[0], offset);
			if(offset >= 0)
			memcpy(&buf[offset], temp, sizeof(int)*1000);
			count++;
		}	

		if(count >= atoi(argv[2])/8)
			break;
	}



	write(fd, buf, bufsz);
	printf("io%d write done\n", atoi(argv[1]));	

	close(fd);
	close(cmd0);
	close(cmd1);
	close(cmd2);
	close(cmd3);
	close(data0);
	close(data1);
	close(data2);
	close(data3);
}
