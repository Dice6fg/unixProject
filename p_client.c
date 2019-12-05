#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<string.h>
#include<malloc.h>

int main(int argc, char *argv[])
{
    /*
    * 
    * rfd1 = 64kb에서 씀, 파일에서 여기로 불러와 ptr에 저장
    * rfd1 = 256kb
    * ptr1 = 블록을 나눠서 저장
    * ptr1_t = client oriented, io
    * io = io node에 전송할때 쓸 파이프를 열 파일 기술자
    */
	int fd, fd1,fd2,fd3,fd4;
	int i,j;
	int rfd1[1000];
	int rfd2[1000];
	int ptr1[4][1000];
    int ptr1_t[4][1000];
	int temp1[1000];
	int ptr2[16][1000];
    int ptr2_t[16][1000];
	int need1[4];
	int need2[16];
	int sigCheck1[3][4];
	int sigCheck2[3][16];
    int p_1to2, p_1to3, p_1to4,p_2to1, p_2to3, p_2to4,p_3to1, p_3to2, p_3to4,p_4to1, p_4to2, p_4to3;
	int p_1to2_s, p_1to3_s, p_1to4_s,p_2to1_s, p_2to3_s, p_2to4_s,p_3to1_s, p_3to2_s, p_3to4_s,p_4to1_s, p_4to2_s, p_4to3_s;
	int io0, io1;
	char* fifoname;
	int bufsiz;

	int w = 0;
    int b = 0;
	
    int count_need = 0;
	int count_read = 0;


    if (atoi(argv[2]) == 64) 
	{
		bufsiz = 1000;	
		fd = open("data1.txt", O_RDONLY);

		switch (atoi(argv[1]))
		{
			case 0: 
				fd1 = open("computenode1_64.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
				io0 = open("fifo0", O_RDWR);
				io1 = open("fifo4", O_RDWR);

				//data 전송 보낼 것.
				p_1to2 = open("1to2",O_RDWR );
				p_1to3 = open("1to3",O_RDWR );
				p_1to4 = open("1to4",O_RDWR );

				//data 전송 받을 것.
                p_2to1 = open("2to1",O_RDONLY |O_NONBLOCK);
				p_3to1 = open("3to1",O_RDONLY |O_NONBLOCK);
				p_4to1 = open("4to1",O_RDONLY |O_NONBLOCK);

				// 시그널 보낼것
                p_1to2_s = open("1to2_s",O_RDWR);
				p_1to3_s = open("1to3_s",O_RDWR);
				p_1to4_s = open("1to4_s",O_RDWR);
				// 시그널 받을것
				p_2to1_s = open("2to1_s",O_RDONLY);
				p_3to1_s = open("3to1_s",O_RDONLY);
				p_4to1_s = open("4to1_s",O_RDONLY);

                //블록사이즈로 나눠서 저장
				for (i = 0; i < 4 ;i++)
				{
					lseek(fd, sizeof(int)*bufsiz*2*i, SEEK_SET);
					
					read(fd, rfd1, sizeof(rfd1));
					memcpy(ptr1[i], rfd1,sizeof(int) * bufsiz);
					               
					write(fd1,ptr1[i],(sizeof(int) * bufsiz));
					
				}
	
			
				//필요한 정보 넣기 1
				need1[0] = 0;
				need1[1] = 10000;
				need1[2] = 20000;
				need1[3] = 30000;

				//각 client에 시그널 전송.
				for(i = 0; i < 4;i++)
				{
                    w = write(p_1to2_s, &need1[i], sizeof(int));				
					w = write(p_1to3_s, &need1[i], sizeof(int));				
					w = write(p_1to4_s, &need1[i], sizeof(int));			
				}
				
                //시그널을 받기
                for(j = 0; j < 4; j++)
				{
					w = read(p_2to1_s, &sigCheck1[0][j], sizeof(int));				
					w = read(p_3to1_s, &sigCheck1[1][j], sizeof(int));						
					w = read(p_4to1_s, &sigCheck1[2][j], sizeof(int));			
				}		
			  
				//데이터 있는지 확인 후 전송
				for(i = 0; i < 4; i++)
				{										
					for(j = 0; j < 4; j++)
					{										
						if(sigCheck1[0][i] == ptr1[j][0]) 
						{
							w = write(p_1to2, ptr1[j], sizeof(int) * 1000);									
						}
						if(sigCheck1[1][i] == ptr1[j][0])
						{
							w = write(p_1to3, ptr1[j], sizeof(int)* 1000);								
						}  
						if(sigCheck1[2][i] == ptr1[j][0])  
						{
							w = write(p_1to4, ptr1[j], sizeof(int)* 1000);
						}
					}
				}			
               	//자기거 체크
				for(i = 0; i < 4; i++)
                {
					for(j = 0; j < 4; j++)
					{
						if(need1[i] == ptr1[j][0]) 
						{
							memcpy(ptr1_t[i], ptr1[j], sizeof(int) * 1000);
							count_need++;
						}
					}
                }
				 //각 client에서 데이터 모으기 
              	while(1)
                {
                    if(b = read(p_2to1, temp1, sizeof(int)* 1000 ) > 0) 
					{						
						for(i = 0; i < 4;i++)
						{
							if(need1[i] == temp1[0]) memcpy(ptr1_t[i], temp1, sizeof(int) * 1000);
						}
						count_read++;
					}
					//printf("%d\n",b);
					if(read(p_3to1, temp1, sizeof(int)* 1000 ) > 0)
					{						
						for(i = 0; i < 4;i++)
						{
							if(need1[i] == temp1[0]) memcpy(ptr1_t[i], temp1, sizeof(int) * 1000);
						}
						count_read++;
					}
					if(read(p_4to1, temp1, sizeof(int)* 1000 ) > 0) 
					{					
						for(i = 0; i < 4;i++)
						{
							if(need1[i] == temp1[0]) memcpy(ptr1_t[i], temp1, sizeof(int) * 1000);
						}
						count_read++;
					}					
					if((4-count_need) == count_read) break;					
                }      
                
                //각 필요한 io로 전송
                w = write(io0, ptr1_t[0], sizeof(int)* 1000);				
				w = write(io1, ptr1_t[1], sizeof(int)* 1000);		
				w = write(io0, ptr1_t[2], sizeof(int)* 1000);			
				w = write(io1, ptr1_t[3], sizeof(int)* 1000);
							
                close(p_1to2);
                close(p_1to3);
				close(p_1to4);
				close(p_2to1);
                close(p_3to1);
				close(p_4to1);
				close(p_1to2_s);
                close(p_1to3_s);
				close(p_1to4_s);
				close(p_2to1_s);
				close(p_3to1_s);
				close(p_4to1_s);
				close(fd1);
				close(io0);
				close(io1);
				break;
			case 1: 
				fd1 = open("computenode2_64.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
				io0 = open("fifo1", O_RDWR);
				io1 = open("fifo5", O_RDWR);

                //data 전송 보낼 것.
				p_2to1 = open("2to1",O_RDWR );
				p_2to3 = open("2to3",O_RDWR );
				p_2to4 = open("2to4",O_RDWR );

				//data 전송 받을 것.
                p_1to2 = open("1to2",O_RDONLY |O_NONBLOCK);
				p_3to2 = open("3to2",O_RDONLY |O_NONBLOCK);
				p_4to2 = open("4to2",O_RDONLY |O_NONBLOCK);

				// 시그널 보낼것
                p_2to1_s = open("2to1_s",O_RDWR);
				p_2to3_s = open("2to3_s",O_RDWR);
				p_2to4_s = open("2to4_s",O_RDWR);
				// 시그널 받을것
				p_1to2_s = open("1to2_s",O_RDONLY);
				p_3to2_s = open("3to2_s",O_RDONLY);
				p_4to2_s = open("4to2_s",O_RDONLY);
              
                //블록사이즈로 나눠서 저장
				
				for (i = 0; i < 4; i++)
				{
					lseek(fd, sizeof(int)*bufsiz*((2*i)+1) , SEEK_SET);
					read(fd, rfd1, sizeof(rfd1));
					for (j = 0; j < bufsiz; j++)
					{
						ptr1[i][j] = rfd1[j];
					}
					write(fd1, rfd1, sizeof(rfd1));
					
				}
				//필요한 정보 넣기 2
				need1[0] = 40000;
				need1[1] = 50000;
				need1[2] = 60000;
				need1[3] = 70000;

				//각 client에 시그널 전송.
				for(i = 0; i < 4;i++)
				{
                    write(p_2to1_s, &need1[i], sizeof(int));
					write(p_2to3_s, &need1[i], sizeof(int));
					write(p_2to4_s, &need1[i], sizeof(int));
				}
                //시그널을 받기
                for(j = 0; j < 4; j++)
				{
					read(p_1to2_s, &sigCheck1[0][j], sizeof(int));
					read(p_3to2_s, &sigCheck1[1][j], sizeof(int));
					read(p_4to2_s, &sigCheck1[2][j], sizeof(int));
				}			  
				//데이터 있는지 확인 후 전송
				for(i = 0; i < 4; i++)
				{										
					for(j = 0; j < 4; j++)
					{										
						if(sigCheck1[0][i] == ptr1[j][0]) 
						{
							w = write(p_2to1, ptr1[j], sizeof(int) * 1000);						
						}
						if(sigCheck1[1][i] == ptr1[j][0])
						{
							w = write(p_2to3, ptr1[j], sizeof(int)* 1000);							
						}  
						if(sigCheck1[2][i] == ptr1[j][0])  
						{
							w = write(p_2to4, ptr1[j], sizeof(int)* 1000);						
						}
					}
				}
               	//자기거 체크
				for(i = 0; i < 4; i++)
                {
					for(j = 0; j < 4; j++)
					{
						if(need1[i] == ptr1[j][0]) 
						{
							memcpy(ptr1_t[i], ptr1[j], sizeof(int) * 1000);
							count_need++;
						}
					}
                }
				 //각 client에서 데이터 모으기                 
              	while(1)
                {
                    if(read(p_1to2, temp1, sizeof(int)* 1000 ) > 0) 
					{
						for(i = 0; i < 4;i++)
						{
							if(need1[i] == temp1[0]) memcpy(ptr1_t[i], temp1, sizeof(int) * 1000);
						}
						count_read++;
					}
					if(read(p_3to2, temp1, sizeof(int)* 1000 ) > 0)
					{
						for(i = 0; i < 4;i++)
						{
							if(need1[i] == temp1[0]) memcpy(ptr1_t[i], temp1, sizeof(int) * 1000);
						}
						count_read++;
					}
					if(read(p_4to2, temp1, sizeof(int)* 1000 ) > 0) 
					{
						for(i = 0; i < 4;i++)
						{
							if(need1[i] == temp1[0]) memcpy(ptr1_t[i], temp1, sizeof(int) * 1000);
						}
						count_read++;
					}
					if((4-count_need) == count_read) break;					
                }      
                
                //각 필요한 io로 전송
                write(io0, ptr1_t[0], sizeof(int)* 1000);
				write(io1, ptr1_t[1], sizeof(int)* 1000);
				write(io0, ptr1_t[2], sizeof(int)* 1000);
				write(io1, ptr1_t[3], sizeof(int)* 1000);
				
                close(p_2to1);
                close(p_2to3);
				close(p_2to4);
				close(p_1to2);
                close(p_3to2);
				close(p_4to2);
				close(p_2to1_s);
                close(p_2to3_s);
				close(p_2to4_s);
				close(p_1to2_s);
				close(p_3to2_s);
				close(p_4to2_s);
				close(fd1);
				close(io0);
				close(io1);
				break;
			case 2: 
				fd1 = open("computenode3_64.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
				io0 = open("fifo2", O_RDWR);
				io1 = open("fifo6", O_RDWR);

				//data 전송 보낼 것.
				p_3to1 = open("3to1",O_RDWR );
				p_3to2 = open("3to2",O_RDWR );
				p_3to4 = open("3to4",O_RDWR );

				//data 전송 받을 것.
                p_1to3 = open("1to3",O_RDONLY |O_NONBLOCK);
				p_2to3 = open("2to3",O_RDONLY |O_NONBLOCK);
				p_4to3 = open("4to3",O_RDONLY |O_NONBLOCK);

				// 시그널 보낼것
                p_3to1_s = open("3to1_s",O_RDWR);
				p_3to2_s = open("3to2_s",O_RDWR);
				p_3to4_s = open("3to4_s",O_RDWR);
				// 시그널 받을것
				p_1to3_s = open("1to3_s",O_RDONLY);
				p_2to3_s = open("2to3_s",O_RDONLY);
				p_4to3_s = open("4to3_s",O_RDONLY);
              
                //블록사이즈로 나눠서 저장
		
				for (i = 0; i < 4; i++)
				{
					lseek(fd, sizeof(int) * bufsiz * ((2 * i) + 8), SEEK_SET);
					read(fd, rfd1, sizeof(rfd1));
					for (j = 0; j < bufsiz; j++)
					{
						ptr1[i][j] = rfd1[j];
					}
					write(fd1, rfd1, sizeof(rfd1));

				}
				//필요한 정보 넣기 3
				need1[0] = 80000;
				need1[1] = 90000;
				need1[2] = 100000;
				need1[3] = 110000;

				//각 client에 시그널 전송.
				for(i = 0; i < 4;i++)
				{
                    write(p_3to1_s, &need1[i], sizeof(int));
					write(p_3to2_s, &need1[i], sizeof(int));
					write(p_3to4_s, &need1[i], sizeof(int));
				}
                //시그널을 받기	
                for(j = 0; j < 4; j++)
				{
					read(p_1to3_s, &sigCheck1[0][j], sizeof(int));
					read(p_2to3_s, &sigCheck1[1][j], sizeof(int));
					read(p_4to3_s, &sigCheck1[2][j], sizeof(int));
				}			  
				//데이터 있는지 확인 후 전송
				for(i = 0; i < 4; i++)
				{										
					for(j = 0; j < 4; j++)
					{										
						if(sigCheck1[0][i] == ptr1[j][0]) 
						{
							w = write(p_3to1, ptr1[j], sizeof(int) * 1000);
					
						}
						if(sigCheck1[1][i] == ptr1[j][0])
						{
							w = write(p_3to2, ptr1[j], sizeof(int)* 1000);	
					
						}  
						if(sigCheck1[2][i] == ptr1[j][0])  
						{
							w = write(p_3to4, ptr1[j], sizeof(int)* 1000);
					
						}
					}
				}
               	//자기거 체크
				for(i = 0; i < 4; i++)
                {
					for(j = 0; j < 4; j++)
					{
						if(need1[i] == ptr1[j][0]) 
						{
							memcpy(ptr1_t[i], ptr1[j], sizeof(int) * 1000);
							count_need++;
						}
					}
                }
				 //각 client에서 데이터 모으기 
                
              	while(1)
                {
                    if(read(p_1to3, temp1, sizeof(int)* 1000 ) > 0) 
					{
						for(i = 0; i < 4;i++)
						{
							if(need1[i] == temp1[0]) memcpy(ptr1_t[i], temp1, sizeof(int) * 1000);
						}
						count_read++;
					}
					if(read(p_2to3, temp1, sizeof(int)* 1000 ) > 0)
					{
						for(i = 0; i < 4;i++)
						{
							if(need1[i] == temp1[0]) memcpy(ptr1_t[i], temp1, sizeof(int) * 1000);
						}
						count_read++;
					}
					if(read(p_4to3, temp1, sizeof(int)* 1000 ) > 0) 
					{
						for(i = 0; i < 4;i++)
						{
							if(need1[i] == temp1[0]) memcpy(ptr1_t[i], temp1, sizeof(int) * 1000);
						}
						count_read++;
					}
					if((4-count_need) == count_read) break;
                }      
                
                //각 필요한 io로 전송
                write(io0, ptr1_t[0], sizeof(int)* 1000);
				write(io1, ptr1_t[1], sizeof(int)* 1000);			
				write(io0, ptr1_t[2], sizeof(int)* 1000);
				write(io1, ptr1_t[3], sizeof(int)* 1000);
				
                close(p_3to1);
                close(p_3to2);
				close(p_3to4);
				close(p_3to1);
                close(p_3to2);
				close(p_3to4);
				close(p_3to1_s);
                close(p_3to2_s);
				close(p_3to4_s);
				close(p_1to3_s);
				close(p_2to3_s);
				close(p_4to3_s);
				close(fd1);
				close(io0);
				close(io1);
				break;
			case 3:
				fd1 = open("computenode4_64.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
				io0 = open("fifo3", O_RDWR);
				io1 = open("fifo7", O_RDWR);
				
                //data 전송 보낼 것.
				p_4to1 = open("4to1",O_RDWR );
				p_4to2 = open("4to2",O_RDWR );
				p_4to3 = open("4to3",O_RDWR );

				//data 전송 받을 것.
                p_1to4 = open("1to4",O_RDONLY |O_NONBLOCK);
				p_2to4 = open("2to4",O_RDONLY |O_NONBLOCK);
				p_3to4 = open("3to4",O_RDONLY |O_NONBLOCK);

				// 시그널 보낼것
                p_4to1_s = open("4to1_s",O_RDWR);
				p_4to2_s = open("4to2_s",O_RDWR);
				p_4to3_s = open("4to3_s",O_RDWR);
				// 시그널 받을것
				p_1to4_s = open("1to4_s",O_RDONLY);
				p_2to4_s = open("2to4_s",O_RDONLY);
				p_3to4_s = open("3to4_s",O_RDONLY);
              
                //블록사이즈로 나눠서 저장
		
				for (i = 0; i < 4; i++)
				{
					lseek(fd, sizeof(int) * bufsiz * ((2 * i) + 9), SEEK_SET);
					read(fd, rfd1, sizeof(rfd1));
					for (j = 0; j < bufsiz; j++)
					{
						ptr1[i][j] = rfd1[j];
					}
					write(fd1, rfd1, sizeof(rfd1));
				}
				//필요한 정보 넣기 3
				need1[0] = 120000;
				need1[1] = 130000;
				need1[2] = 140000;
				need1[3] = 150000;

				//각 client에 시그널 전송.
				for(i = 0; i < 4;i++)
				{
                    write(p_4to1_s, &need1[i], sizeof(int));
					write(p_4to2_s, &need1[i], sizeof(int));
					write(p_4to3_s, &need1[i], sizeof(int));
				}
                //시그널을 받기
                for(j = 0; j < 4; j++)
				{
					read(p_1to4_s, &sigCheck1[0][j], sizeof(int));
					read(p_2to4_s, &sigCheck1[1][j], sizeof(int));
					read(p_3to4_s, &sigCheck1[2][j], sizeof(int));
					
				}			  
				//데이터 있는지 확인 후 전송
				for(i = 0; i < 4; i++)
				{										
					for(j = 0; j < 4; j++)
					{										
						if(sigCheck1[0][i] == ptr1[j][0]) 
						{
							w = write(p_4to1, ptr1[j], sizeof(int) * 1000);			
							
						}
						if(sigCheck1[1][i] == ptr1[j][0])
						{
							w = write(p_4to2, ptr1[j], sizeof(int)* 1000);	
											
						}  
						if(sigCheck1[2][i] == ptr1[j][0])  
						{
							w = write(p_4to3, ptr1[j], sizeof(int)* 1000);										
						}
					}
				}
               	//자기거 체크
				for(i = 0; i < 4; i++)
                {
					for(j = 0; j < 4; j++)
					{
						if(need1[i] == ptr1[j][0]) 
						{
							memcpy(ptr1_t[i], ptr1[j], sizeof(int) * 1000);
							count_need++;							
						}
					}
                }
				 //각 client에서 데이터 모으기 
                
              	while(1)
                {
                    if(w =read(p_1to4, temp1, sizeof(int)* 1000 ) > 0) 
					{
						for(i = 0; i < 4;i++)
						{
							if(need1[i] == temp1[0]) memcpy(ptr1_t[i], temp1, sizeof(int) * 1000);
						}
						count_read++;
					}
				
					if(read(p_2to4, temp1, sizeof(int)* 1000 ) > 0)
					{
						for(i = 0; i < 4;i++)
						{
							if(need1[i] == temp1[0]) memcpy(ptr1_t[i], temp1, sizeof(int) * 1000);
						}
						count_read++;
					}
					if(read(p_3to4, temp1, sizeof(int)* 1000 ) > 0) 
					{
						for(i = 0; i < 4;i++)
						{
							if(need1[i] == temp1[0]) memcpy(ptr1_t[i], temp1, sizeof(int) * 1000);
						}
						count_read++;
					}
					if((4-count_need) == count_read) break;
		
                }      
                
                //각 필요한 io로 전송
                write(io0, ptr1_t[0], sizeof(int)* 1000);				
				write(io1, ptr1_t[1], sizeof(int)* 1000);				
				write(io0, ptr1_t[2], sizeof(int)* 1000);				
				write(io1, ptr1_t[3], sizeof(int)* 1000);
			

                close(p_3to1);
                close(p_3to2);
				close(p_3to4);
				close(p_3to1);
                close(p_3to2);
				close(p_3to4);
				close(p_3to1_s);
                close(p_3to2_s);
				close(p_3to4_s);
				close(p_1to3_s);
				close(p_2to3_s);
				close(p_4to3_s);
				close(fd1);
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
				io0 = open("fifo0", O_RDWR);
				io1 = open("fifo4", O_RDWR);

				//data 전송 보낼 것.
				p_1to2 = open("1to2",O_RDWR );
				p_1to3 = open("1to3",O_RDWR );
				p_1to4 = open("1to4",O_RDWR );

				//data 전송 받을 것.
                p_2to1 = open("2to1",O_RDONLY |O_NONBLOCK);
				p_3to1 = open("3to1",O_RDONLY |O_NONBLOCK);
				p_4to1 = open("4to1",O_RDONLY |O_NONBLOCK);

				// 시그널 보낼것
                p_1to2_s = open("1to2_s",O_RDWR);
				p_1to3_s = open("1to3_s",O_RDWR);
				p_1to4_s = open("1to4_s",O_RDWR);
				// 시그널 받을것
				p_2to1_s = open("2to1_s",O_RDONLY);
				p_3to1_s = open("3to1_s",O_RDONLY);
				p_4to1_s = open("4to1_s",O_RDONLY);
				printf("asd");
                //블록사이즈로 나눠서 저장				
			
				for (i = 0; i < 8; i++)
				{
					lseek(fd, sizeof(int)*bufsiz*4*i, SEEK_SET);
					for(j = 0; j < 2; j++)
					{
						read(fd, rfd2, sizeof(rfd2));
						memcpy(ptr2[(i*2) + j], rfd2,sizeof(int) * bufsiz);
						write(fd1, rfd2, sizeof(rfd2));
					}               
				}
				
				//필요한 정보 넣기 1
				for (i = 0; i < 16; i++)
				{
					need2[i] = i * 10000;
				}

				//각 client에 시그널 전송.
				for(i = 0; i < 16;i++)
				{
                    write(p_1to2_s, &need2[i], sizeof(int));
					write(p_1to3_s, &need2[i], sizeof(int));
					write(p_1to4_s, &need2[i], sizeof(int));
				}
                //시그널을 받기
                for(j = 0; j < 16; j++)
				{
					read(p_2to1_s, &sigCheck2[0][j], sizeof(int));
					read(p_3to1_s, &sigCheck2[1][j], sizeof(int));
					read(p_4to1_s, &sigCheck2[2][j], sizeof(int));
				}			  
				//데이터 있는지 확인 후 전송		
				for(i = 0; i < 16; i++)
				{										
					for(j = 0; j < 16; j++)
					{										
						if(sigCheck2[0][i] == ptr2[j][0]) 
						{
							w = write(p_1to2, ptr2[j], sizeof(int) * 1000);									
						}
						if(sigCheck2[1][i] == ptr2[j][0])
						{
							w = write(p_1to3, ptr2[j], sizeof(int)* 1000);								
						}  
						if(sigCheck2[2][i] == ptr2[j][0])  
						{
							w = write(p_1to4, ptr2[j], sizeof(int)* 1000);
						}
					}
				}			
               	//자기거 체크			
				for(i = 0; i < 16; i++)
                {
					for(j = 0; j < 16; j++)
					{
						if(need2[i] == ptr2[j][0]) 
						{
							memcpy(ptr2_t[i], ptr2[j], sizeof(int) * 1000);
							count_need++;
						}
					}
                }
				 //각 client에서 데이터 모으기 
                
              	while(1)
                {
                    if(read(p_2to1, temp1, sizeof(int)* 1000 ) > 0) 
					{
						for(i = 0; i < 16;i++)
						{
							if(need2[i] == temp1[0]) memcpy(ptr2_t[i], temp1, sizeof(int) * 1000);
						}
						count_read++;
					}
					if(read(p_3to1, temp1, sizeof(int)* 1000 ) > 0)
					{
						for(i = 0; i < 16;i++)
						{
							if(need2[i] == temp1[0]) memcpy(ptr2_t[i], temp1, sizeof(int) * 1000);
						}
						count_read++;
					}
					if(read(p_4to1, temp1, sizeof(int)* 1000 ) > 0) 
					{
						for(i = 0; i < 16;i++)
						{
							if(need2[i] == temp1[0]) memcpy(ptr2_t[i], temp1, sizeof(int) * 1000);
						}
						count_read++;
					}
					if((16-count_need) == count_read) break;
                }      
                
                //각 필요한 io로 전송
				for(i = 0 ; i < 8; i++)
				{
                w = write(io0, ptr2_t[(i*2)], sizeof(int)* 1000);
				printf("1!!; : %d\n", w);
				w = write(io1, ptr2_t[(i*2)+1], sizeof(int)* 1000);
				printf("1!!; : %d\n", w);
				}
				
                close(p_1to2);
                close(p_1to3);
				close(p_1to4);
				close(p_2to1);
                close(p_3to1);
				close(p_4to1);
				close(p_1to2_s);
                close(p_1to3_s);
				close(p_1to4_s);
				close(p_2to1_s);
				close(p_3to1_s);
				close(p_4to1_s);
				close(fd1);
				close(io0);
				close(io1);
				break;
			case 1: 
				fd1 = open("computenode2_256.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
				io0 = open("fifo1", O_RDWR);
				io1 = open("fifo5", O_RDWR);

                //data 전송 보낼 것.
				p_2to1 = open("2to1",O_RDWR );
				p_2to3 = open("2to3",O_RDWR );
				p_2to4 = open("2to4",O_RDWR );

				//data 전송 받을 것.
                p_1to2 = open("1to2",O_RDONLY |O_NONBLOCK);
				p_3to2 = open("3to2",O_RDONLY |O_NONBLOCK);
				p_4to2 = open("4to2",O_RDONLY |O_NONBLOCK);

				// 시그널 보낼것
                p_2to1_s = open("2to1_s",O_RDWR);
				p_2to3_s = open("2to3_s",O_RDWR);
				p_2to4_s = open("2to4_s",O_RDWR);
				// 시그널 받을것
				p_1to2_s = open("1to2_s",O_RDONLY);
				p_3to2_s = open("3to2_s",O_RDONLY);
				p_4to2_s = open("4to2_s",O_RDONLY);
              
                //블록사이즈로 나눠서 저장
		
				for (i = 0; i < 8; i++)
				{
					lseek(fd, sizeof(int)*bufsiz*4*i + sizeof(int)*bufsiz*2, SEEK_SET);

					for(j = 0; j < 2; j++)
					{
						read(fd, rfd2, sizeof(rfd2));
						memcpy(ptr2[(i*2) + j], rfd2,sizeof(int) * bufsiz);
						write(fd1, rfd2, sizeof(rfd2));
					}               
				}
				//필요한 정보 넣기 2
				for (i = 0; i < 16; i++)
				{
					need2[i] = (i+16) * 10000;
				}

				//각 client에 시그널 전송.
				for(i = 0; i < 16;i++)
				{
                    write(p_2to1_s, &need2[i], sizeof(int));
					write(p_2to3_s, &need2[i], sizeof(int));
					write(p_2to4_s, &need2[i], sizeof(int));
				}
                //시그널을 받기
                for(j = 0; j < 16; j++)
				{
					read(p_1to2_s, &sigCheck2[0][j], sizeof(int));
					read(p_3to2_s, &sigCheck2[1][j], sizeof(int));
					read(p_4to2_s, &sigCheck2[2][j], sizeof(int));
				}			  
				//데이터 있는지 확인 후 전송		
				for(i = 0; i < 16; i++)
				{										
					for(j = 0; j < 16; j++)
					{										
						if(sigCheck2[0][i] == ptr2[j][0]) 
						{
							w = write(p_2to1, ptr2[j], sizeof(int) * 1000);									
						}
						if(sigCheck2[1][i] == ptr2[j][0])
						{
							w = write(p_2to3, ptr2[j], sizeof(int)* 1000);								
						}  
						if(sigCheck2[2][i] == ptr2[j][0])  
						{
							w = write(p_2to4, ptr2[j], sizeof(int)* 1000);
						}
					}
				}			
               	//자기거 체크			
				for(i = 0; i < 16; i++)
                {
					for(j = 0; j < 16; j++)
					{
						if(need2[i] == ptr2[j][0]) 
						{
							memcpy(ptr2_t[i], ptr2[j], sizeof(int) * 1000);
							count_need++;
						}
					}
                }				
				 //각 client에서 데이터 모으기                 
              	while(1)
                {
                    if(read(p_1to2, temp1, sizeof(int)* 1000 ) > 0) 
					{
						for(i = 0; i < 16;i++)
						{
							if(need2[i] == temp1[0]) memcpy(ptr2_t[i], temp1, sizeof(int) * 1000);
						}
						count_read++;
					}
					if(read(p_3to2, temp1, sizeof(int)* 1000 ) > 0)
					{
						for(i = 0; i < 16;i++)
						{
							if(need2[i] == temp1[0]) memcpy(ptr2_t[i], temp1, sizeof(int) * 1000);
						}
						count_read++;
					}
					if(read(p_4to2, temp1, sizeof(int)* 1000 ) > 0) 
					{
						for(i = 0; i < 16;i++)
						{
							if(need2[i] == temp1[0]) memcpy(ptr2_t[i], temp1, sizeof(int) * 1000);
						}
						count_read++;
					}
					if((16-count_need) == count_read) break;
                }      
                
                //각 필요한 io로 전송
               	for(i = 0 ; i < 8; i++)
				{
                w = write(io0, ptr2_t[(i*2)], sizeof(int)* 1000);
				printf("1!!; : %d\n", w);
				w = write(io1, ptr2_t[(i*2)+1], sizeof(int)* 1000);
				printf("1!!; : %d\n", w);
				}
				
                close(p_2to1);
                close(p_2to3);
				close(p_2to4);
				close(p_1to2);
                close(p_3to2);
				close(p_4to2);
				close(p_2to1_s);
                close(p_2to3_s);
				close(p_2to4_s);
				close(p_1to2_s);
				close(p_3to2_s);
				close(p_4to2_s);
				close(fd1);
				close(io0);
				close(io1);
				break;
			case 2: 
				fd1 = open("computenode3_256.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
				io0 = open("fifo2", O_RDWR);
				io1 = open("fifo6", O_RDWR);

				//data 전송 보낼 것.
				p_3to1 = open("3to1",O_RDWR );
				p_3to2 = open("3to2",O_RDWR );
				p_3to4 = open("3to4",O_RDWR );

				//data 전송 받을 것.
                p_1to3 = open("1to3",O_RDONLY |O_NONBLOCK);
				p_2to3 = open("2to3",O_RDONLY |O_NONBLOCK);
				p_4to3 = open("4to3",O_RDONLY |O_NONBLOCK);

				// 시그널 보낼것
                p_3to1_s = open("3to1_s",O_RDWR);
				p_3to2_s = open("3to2_s",O_RDWR);
				p_3to4_s = open("3to4_s",O_RDWR);
				// 시그널 받을것
				p_1to3_s = open("1to3_s",O_RDONLY);
				p_2to3_s = open("2to3_s",O_RDONLY);
				p_4to3_s = open("4to3_s",O_RDONLY);
              
                //블록사이즈로 나눠서 저장
				
				for (i = 0; i < 8; i++)
				{
					lseek(fd, sizeof(int)*bufsiz*4*i + sizeof(int)*bufsiz*32, SEEK_SET);

					for(j = 0; j < 2; j++)
					{
						read(fd, rfd2, sizeof(rfd2));
						memcpy(ptr2[(i*2) + j], rfd2,sizeof(int) * bufsiz);
						write(fd1, rfd2, sizeof(rfd2));
					}               
				}
				//필요한 정보 넣기 3
				for (i = 0; i < 16; i++)
				{
					need2[i] = (i+32) * 10000;
				}

				//각 client에 시그널 전송.
				for(i = 0; i < 16;i++)
				{
                    write(p_3to1_s, &need2[i], sizeof(int));
					write(p_3to2_s, &need2[i], sizeof(int));
					write(p_3to4_s, &need2[i], sizeof(int));
				}
                //시그널을 받기	
                for(j = 0; j < 16; j++)
				{
					read(p_1to3_s, &sigCheck2[0][j], sizeof(int));
					read(p_2to3_s, &sigCheck2[1][j], sizeof(int));
					read(p_4to3_s, &sigCheck2[2][j], sizeof(int));
				}			  
				//데이터 있는지 확인 후 전송		
				for(i = 0; i < 16; i++)
				{										
					for(j = 0; j < 16; j++)
					{										
						if(sigCheck2[0][i] == ptr2[j][0]) 
						{
							w = write(p_3to1, ptr2[j], sizeof(int) * 1000);									
						}
						if(sigCheck2[1][i] == ptr2[j][0])
						{
							w = write(p_3to2, ptr2[j], sizeof(int)* 1000);								
						}  
						if(sigCheck2[2][i] == ptr2[j][0])  
						{
							w = write(p_3to4, ptr2[j], sizeof(int)* 1000);
						}
					}
				}			
               	//자기거 체크			
				for(i = 0; i < 16; i++)
                {
					for(j = 0; j < 16; j++)
					{
						if(need2[i] == ptr2[j][0]) 
						{
							memcpy(ptr2_t[i], ptr2[j], sizeof(int) * 1000);
							count_need++;
						}
					}
                }
				 //각 client에서 데이터 모으기 
                
              	while(1)
                {
                    if(read(p_1to3, temp1, sizeof(int)* 1000 ) > 0) 
					{
						for(i = 0; i < 16;i++)
						{
							if(need2[i] == temp1[0]) memcpy(ptr2_t[i], temp1, sizeof(int) * 1000);
						}
						count_read++;
					}
					if(read(p_2to3, temp1, sizeof(int)* 1000 ) > 0)
					{
						for(i = 0; i < 16;i++)
						{
							if(need2[i] == temp1[0]) memcpy(ptr2_t[i], temp1, sizeof(int) * 1000);
						}
						count_read++;
					}
					if(read(p_4to3, temp1, sizeof(int)* 1000 ) > 0) 
					{
						for(i = 0; i < 16;i++)
						{
							if(need2[i] == temp1[0]) memcpy(ptr2_t[i], temp1, sizeof(int) * 1000);
						}
						count_read++;
					}
					if((16-count_need) == count_read) break;
                }      
                
                //각 필요한 io로 전송
                for(i = 0 ; i < 8; i++)
				{
                w = write(io0, ptr2_t[(i*2)], sizeof(int)* 1000);
				printf("1!!; : %d\n", w);
				w = write(io1, ptr2_t[(i*2)+1], sizeof(int)* 1000);
				printf("1!!; : %d\n", w);
				}
				
                close(p_3to1);
                close(p_3to2);
				close(p_3to4);
				close(p_3to1);
                close(p_3to2);
				close(p_3to4);
				close(p_3to1_s);
                close(p_3to2_s);
				close(p_3to4_s);
				close(p_1to3_s);
				close(p_2to3_s);
				close(p_4to3_s);
				close(fd1);
				close(io0);
				close(io1);
				break;
			case 3:
				io0 = open("fifo3", O_RDWR);
				io1 = open("fifo7", O_RDWR);
				fd1 = open("computenode4_256.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);

                //data 전송 보낼 것.
				p_4to1 = open("4to1",O_RDWR );
				p_4to2 = open("4to2",O_RDWR );
				p_4to3 = open("4to3",O_RDWR );

				//data 전송 받을 것.
                p_1to4 = open("1to4",O_RDONLY |O_NONBLOCK);
				p_2to4 = open("2to4",O_RDONLY |O_NONBLOCK);
				p_3to4 = open("3to4",O_RDONLY |O_NONBLOCK);

				// 시그널 보낼것
                p_4to1_s = open("4to1_s",O_RDWR);
				p_4to2_s = open("4to2_s",O_RDWR);
				p_4to3_s = open("4to3_s",O_RDWR);
				// 시그널 받을것
				p_1to4_s = open("1to4_s",O_RDONLY);
				p_2to4_s = open("2to4_s",O_RDONLY);
				p_3to4_s = open("3to4_s",O_RDONLY);
              
                //블록사이즈로 나눠서 저장
			
				for (i = 0; i < 8; i++)
				{
					lseek(fd, sizeof(int)*bufsiz*4*i + sizeof(int)*bufsiz*34, SEEK_SET);

					for(j = 0; j < 2; j++)
					{
						read(fd, rfd2, sizeof(rfd2));
						memcpy(ptr2[(i*2) + j], rfd2,sizeof(int) * bufsiz);
						write(fd1, rfd2, sizeof(rfd2));
					}               
				}
				//필요한 정보 넣기 4
				for (i = 0; i < 16; i++)
				{
					need2[i] = (i+48) * 10000;
				}

				//각 client에 시그널 전송.
				for(i = 0; i < 16;i++)
				{
                    write(p_4to1_s, &need2[i], sizeof(int));
					write(p_4to2_s, &need2[i], sizeof(int));
					write(p_4to3_s, &need2[i], sizeof(int));
				}
                //시그널을 받기	
                for(j = 0; j < 16; j++)
				{
					read(p_1to4_s, &sigCheck2[0][j], sizeof(int));
					read(p_2to4_s, &sigCheck2[1][j], sizeof(int));
					read(p_3to4_s, &sigCheck2[2][j], sizeof(int));
				}			  
				//데이터 있는지 확인 후 전송		
				for(i = 0; i < 16; i++)
				{										
					for(j = 0; j < 16; j++)
					{										
						if(sigCheck2[0][i] == ptr2[j][0]) 
						{
							w = write(p_4to1, ptr2[j], sizeof(int) * 1000);									
						}
						if(sigCheck2[1][i] == ptr2[j][0])
						{
							w = write(p_4to2, ptr2[j], sizeof(int)* 1000);								
						}  
						if(sigCheck2[2][i] == ptr2[j][0])  
						{
							w = write(p_4to3, ptr2[j], sizeof(int)* 1000);
						}
					}
				}			
               	//자기거 체크			
				for(i = 0; i < 16; i++)
                {
					for(j = 0; j < 16; j++)
					{
						if(need2[i] == ptr2[j][0]) 
						{
							memcpy(ptr2_t[i], ptr2[j], sizeof(int) * 1000);
							count_need++;
						}
					}
                }
				 //각 client에서 데이터 모으기 
                
              	while(1)
                {
                    if(read(p_1to4, temp1, sizeof(int)* 1000 ) > 0) 
					{
						for(i = 0; i < 16;i++)
						{
							if(need2[i] == temp1[0]) memcpy(ptr2_t[i], temp1, sizeof(int) * 1000);
						}
						count_read++;
					}
					if(read(p_2to4, temp1, sizeof(int)* 1000 ) > 0)
					{
						for(i = 0; i < 16;i++)
						{
							if(need2[i] == temp1[0]) memcpy(ptr2_t[i], temp1, sizeof(int) * 1000);
						}
						count_read++;
					}
					if(read(p_3to4, temp1, sizeof(int)* 1000 ) > 0) 
					{
						for(i = 0; i < 16;i++)
						{
							if(need2[i] == temp1[0]) memcpy(ptr2_t[i], temp1, sizeof(int) * 1000);
						}
						count_read++;
					}
					if((16-count_need) == count_read) break;
                }      
                
                //각 필요한 io로 전송
                for(i = 0 ; i < 8; i++)
				{
                w = write(io0, ptr2_t[(i*2)], sizeof(int)* 1000);
				printf("1!!; : %d\n", w);
				w = write(io1, ptr2_t[(i*2)+1], sizeof(int)* 1000);
				printf("1!!; : %d\n", w);
				}
				
                close(p_3to1);
                close(p_3to2);
				close(p_3to4);
				close(p_3to1);
                close(p_3to2);
				close(p_3to4);
				close(p_3to1_s);
                close(p_3to2_s);
				close(p_3to4_s);
				close(p_1to3_s);
				close(p_2to3_s);
				close(p_4to3_s);
				close(fd1);
				close(io0);
				close(io1);
				break;

			default: break;
		}
		close(fd);
	}
}