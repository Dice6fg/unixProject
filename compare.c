#include <stdio.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
	int fd1,fd2,num;
	int num1[64000];
	int num2[64000];
	int count = 0;
	int i =0;
	fd1 = open(argv[1], O_RDONLY);
	fd2 = open(argv[2], O_RDONLY);
	while(read(fd1, &num, sizeof(int)) > 0)
	{
		num1[count] = num;
		count++;
	}
	
	count = 0;
	
	while(read(fd2, &num, sizeof(int)) > 0)
	{
		num2[count] = num;
		count++;
	}
	
	while(i<count)
	{
		if(num1[i] != num2[i]) printf("wrong different position is : %d and num1 : %d num2 : %d\n",i,num1[i],num2[i]);
		i++;	
	}
	return 0;
}
