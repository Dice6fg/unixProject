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
	int ptr1[4000];
    int ptr1_t[4000];
    int ptr1_s[4000];
	int temp1[1000];
	int ptr2[16000];
    int ptr2_t[16000];
    int ptr2_s[16000];
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
				fd1 = open("computenode1_64_c.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
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
				for (i = 0; i < 4000 ;i++)
				{
					lseek(fd, sizeof(int)*4*i, SEEK_SET);
					
					read(fd, ptr1 +  i, sizeof(int));
					//memcpy(ptr1[i], rfd1,sizeof(int) * bufsiz);			               
				}
                write(fd1,ptr1,(sizeof(int) * 4000));			
	
			
				//필요한 정보 넣기 1
				need1[0] = 0;
				need1[1] = 10;
				need1[2] = 20;
				need1[3] = 30;

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
						if(sigCheck1[0][i] == ptr1[1000 * j]) 
						{
							w = write(p_1to2, ptr1 + 1000 * j, sizeof(int) * 1000);									
						}
						if(sigCheck1[1][i] == ptr1[1000 * j])
						{
							w = write(p_1to3, ptr1 + 1000 * j, sizeof(int)* 1000);								
						}  
						if(sigCheck1[2][i] == ptr1[1000 * j]) 
						{
							w = write(p_1to4, ptr1 + 1000 * j, sizeof(int)* 1000);
						}
					}
				}			
               	//자기거 체크
				for(i = 0; i < 4; i++)
                {
					for(j = 0; j < 4; j++)
					{
						if(need1[i] == ptr1[1000 * j]) 
						{
							memcpy(ptr1_t + 1000 * i, ptr1 +  1000 * j, sizeof(int) * 1000);
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
							if(need1[i] == temp1[0]) memcpy(ptr1_t +  1000 * i, temp1, sizeof(int) * 1000);
						}
						count_read++;
					}
					//printf("%d\n",b);
					if(read(p_3to1, temp1, sizeof(int)* 1000 ) > 0)
					{						
						for(i = 0; i < 4;i++)
						{
							if(need1[i] == temp1[0]) memcpy(ptr1_t + 1000 * i, temp1, sizeof(int) * 1000);
						}
						count_read++;
					}
					if(read(p_4to1, temp1, sizeof(int)* 1000 ) > 0) 
					{					
						for(i = 0; i < 4;i++)
						{
							if(need1[i] == temp1[0]) memcpy(ptr1_t +  1000 * i, temp1, sizeof(int) * 1000);
						}
						count_read++;
					}					
					if((4-count_need) == count_read) break;					
                }      
                //정렬
                for(i = 0; i < 4000; i++)
                {
                   w = ptr1_t[i] / 10;
                   ptr1_s[w] = ptr1_t[i];
                }
                
                //각 필요한 io로 전송
                w = write(io0, ptr1_s, sizeof(int)* 1000);				
				w = write(io1, ptr1_s + 1000, sizeof(int)* 1000);		
				w = write(io0, ptr1_s + 2000, sizeof(int)* 1000);			
				w = write(io1, ptr1_s + 3000, sizeof(int)* 1000);
							
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
				fd1 = open("computenode2_64_c.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
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
				for (i = 0; i < 4000 ;i++)
				{
					lseek(fd, sizeof(int)*4*i + sizeof(int) * 1, SEEK_SET);
					
					read(fd, ptr1 + i, sizeof(int));
					//memcpy(ptr1[i], rfd1,sizeof(int) * bufsiz);			               
				}
          		
	            write(fd1,ptr1,(sizeof(int) * 4000));			

				//필요한 정보 넣기 2
				need1[0] = 40000;
				need1[1] = 40010;
				need1[2] = 40020;
				need1[3] = 40030;

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
						if(sigCheck1[0][i] == ptr1[1000 * j]) 
						{
							w = write(p_2to1, ptr1 +1000 * j, sizeof(int) * 1000);									
						}
						if(sigCheck1[1][i] == ptr1[1000 * j])
						{
							w = write(p_2to3, ptr1 + 1000 * j, sizeof(int)* 1000);								
						}  
						if(sigCheck1[2][i] == ptr1[1000 * j]) 
						{
							w = write(p_2to4, ptr1 +1000 * j, sizeof(int)* 1000);
						}
					}
				}			
               	//자기거 체크
				for(i = 0; i < 4; i++)
                {
					for(j = 0; j < 4; j++)
					{
						if(need1[i] == ptr1[1000 * j]) 
						{
							memcpy(ptr1_t +  1000 * i, ptr1 +  1000 * j, sizeof(int) * 1000);
							count_need++;
						}
					}
                }
				 //각 client에서 데이터 모으기 
              	while(1)
                {
                    if(b = read(p_1to2, temp1, sizeof(int)* 1000 ) > 0) 
					{						
						for(i = 0; i < 4;i++)
						{
							if(need1[i] == temp1[0]) memcpy(ptr1_t + 1000 * i, temp1, sizeof(int) * 1000);
						}
						count_read++;
					}
					//printf("%d\n",b);
					if(read(p_3to2, temp1, sizeof(int)* 1000 ) > 0)
					{						
						for(i = 0; i < 4;i++)
						{
							if(need1[i] == temp1[0]) memcpy(ptr1_t + 1000 * i, temp1, sizeof(int) * 1000);
						}
						count_read++;
					}
					if(read(p_4to2, temp1, sizeof(int)* 1000 ) > 0) 
					{					
						for(i = 0; i < 4;i++)
						{
							if(need1[i] == temp1[0]) memcpy(ptr1_t +  1000 * i, temp1, sizeof(int) * 1000);
						}
						count_read++;
					}					
					if((4-count_need) == count_read) break;		
                   // printf("%d : %d\n",count_need,count_read);
                   		
                }      
                //정렬
                for(i = 0; i < 4000; i++)
                {
                   w = ptr1_t[i] / 10 - 4000;
                   ptr1_s[w] = ptr1_t[i];
                }
                
                //각 필요한 io로 전송
                w = write(io0, ptr1_s, sizeof(int)* 1000);				
				w = write(io1, ptr1_s + 1000, sizeof(int)* 1000);		
				w = write(io0, ptr1_s + 2000, sizeof(int)* 1000);			
				w = write(io1, ptr1_s + 3000, sizeof(int)* 1000);
				
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
				fd1 = open("computenode3_64_c.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
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
		
				//블록사이즈로 나눠서 저장
				for (i = 0; i < 4000 ;i++)
				{
					lseek(fd, sizeof(int)*4*i + sizeof(int) * 2, SEEK_SET);
					
					read(fd, ptr1 +  i, sizeof(int));
					//memcpy(ptr1[i], rfd1,sizeof(int) * bufsiz);			               
				}
                write(fd1,ptr1,(sizeof(int) * 4000));			
				//필요한 정보 넣기 3
				need1[0] = 80000;
				need1[1] = 80010;
				need1[2] = 80020;
				need1[3] = 80030;

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
						if(sigCheck1[0][i] == ptr1[1000 * j]) 
						{
							w = write(p_3to1, ptr1 +  1000 * j, sizeof(int) * 1000);									
						}
						if(sigCheck1[1][i] == ptr1[1000 * j])
						{
							w = write(p_3to2, ptr1 +  1000 * j, sizeof(int)* 1000);								
						}  
						if(sigCheck1[2][i] == ptr1[1000 * j]) 
						{
							w = write(p_3to4, ptr1 + 1000 * j, sizeof(int)* 1000);
						}
					}
				}			
               	//자기거 체크
				for(i = 0; i < 4; i++)
                {
					for(j = 0; j < 4; j++)
					{
						if(need1[i] == ptr1[1000 * j]) 
						{
							memcpy(ptr1_t +  1000 * i, ptr1 + 1000 * j, sizeof(int) * 1000);
							count_need++;
						}
					}
                }
				 //각 client에서 데이터 모으기 
              	while(1)
                {
                    if(b = read(p_1to3, temp1, sizeof(int)* 1000 ) > 0) 
					{						
						for(i = 0; i < 4;i++)
						{
							if(need1[i] == temp1[0]) memcpy(ptr1_t + 1000 * i, temp1, sizeof(int) * 1000);
						}
						count_read++;                        
					}
					//printf("%d\n",b);
					if(read(p_2to3, temp1, sizeof(int)* 1000 ) > 0)
					{						
						for(i = 0; i < 4;i++)
						{
							if(need1[i] == temp1[0]) memcpy(ptr1_t + 1000 * i, temp1, sizeof(int) * 1000);
						}
						count_read++;                        
					}
					if(read(p_4to3, temp1, sizeof(int)* 1000 ) > 0) 
					{					
						for(i = 0; i < 4;i++)
						{
							if(need1[i] == temp1[0]) memcpy(ptr1_t + 1000 * i, temp1, sizeof(int) * 1000);
						}
						count_read++;
                         
					}				                  
					if((4-count_need) == count_read) break;		
                 	
                }      
                //정렬
                for(i = 0; i < 4000; i++)
                {
                   w = ptr1_t[i] / 10 - 8000;
                   ptr1_s[w] = ptr1_t[i];
                }
                
                //각 필요한 io로 전송
                w = write(io0, ptr1_s, sizeof(int)* 1000);				
				w = write(io1, ptr1_s + 1000, sizeof(int)* 1000);		
				w = write(io0, ptr1_s + 2000, sizeof(int)* 1000);			
				w = write(io1, ptr1_s + 3000, sizeof(int)* 1000);
				
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
				fd1 = open("computenode4_64_c.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
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
		
				//블록사이즈로 나눠서 저장
				for (i = 0; i < 4000 ;i++)
				{
					lseek(fd, sizeof(int)*4*i + sizeof(int) * 3, SEEK_SET);
					
					read(fd, ptr1 +  i, sizeof(int));
					//memcpy(ptr1[i], rfd1,sizeof(int) * bufsiz);			               
				}
                write(fd1,ptr1,(sizeof(int) * 4000));			
				//필요한 정보 넣기 3
				need1[0] = 120000;
				need1[1] = 120010;
				need1[2] = 120020;
				need1[3] = 120030;

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
						if(sigCheck1[0][i] == ptr1[1000 * j]) 
						{
							w = write(p_4to1, ptr1 +  1000 * j, sizeof(int) * 1000);									
						}
						if(sigCheck1[1][i] == ptr1[1000 * j])
						{
							w = write(p_4to2, ptr1 +  1000 *j, sizeof(int)* 1000);								
						}  
						if(sigCheck1[2][i] == ptr1[1000 * j]) 
						{
							w = write(p_4to3, ptr1 +  1000 * j, sizeof(int)* 1000);
						}
					}
				}			
               	//자기거 체크
				for(i = 0; i < 4; i++)
                {
					for(j = 0; j < 4; j++)
					{
						if(need1[i] == ptr1[1000 * j]) 
						{
							memcpy(ptr1_t +  1000 * i, ptr1 +  1000 * j, sizeof(int) * 1000);
							count_need++;
						}
					}
                }
				 //각 client에서 데이터 모으기 
              	while(1)
                {
                    if(b = read(p_1to4, temp1, sizeof(int)* 1000 ) > 0) 
					{						
						for(i = 0; i < 4;i++)
						{
							if(need1[i] == temp1[0]) memcpy(ptr1_t + 1000 * i, temp1, sizeof(int) * 1000);
						}
						count_read++;
					}
					//printf("%d\n",b);
					if(read(p_2to4, temp1, sizeof(int)* 1000 ) > 0)
					{						
						for(i = 0; i < 4;i++)
						{
							if(need1[i] == temp1[0]) memcpy(ptr1_t +  1000 * i, temp1, sizeof(int) * 1000);
						}
						count_read++;
					}
					if(read(p_3to4, temp1, sizeof(int)* 1000 ) > 0) 
					{					
						for(i = 0; i < 4;i++)
						{
							if(need1[i] == temp1[0]) memcpy(ptr1_t +  1000 * i, temp1, sizeof(int) * 1000);
						}
						count_read++;
					}					
					if((4-count_need) == count_read) break;					
                }      
                //정렬
                for(i = 0; i < 4000; i++)
                {
                   w = ptr1_t[i] / 10 - 12000;
                   ptr1_s[w] = ptr1_t[i];				  
                }
                
                 //각 필요한 io로 전송
                w = write(io0, ptr1_s, sizeof(int)* 1000);				
				w = write(io1, ptr1_s + 1000, sizeof(int)* 1000);		
				w = write(io0, ptr1_s + 2000, sizeof(int)* 1000);			
				w = write(io1, ptr1_s + 3000, sizeof(int)* 1000);

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
				fd1 = open("computenode1_256_c.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
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
				for (i = 0; i < 16000 ;i++)
				{
					lseek(fd, sizeof(int)*4*i, SEEK_SET);
					
					read(fd, ptr2 + i, sizeof(int));
					//memcpy(ptr1[i], rfd1,sizeof(int) * bufsiz);			               
				}
                write(fd1,ptr2,(sizeof(int) * 16000));			
				
				//필요한 정보 넣기 1
				for (i = 0; i < 4; i++)
				{
					for(j = 0; j< 4; j++)
					need2[i *4 +j] = j * 10 + i * 40000;
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
						if(sigCheck2[0][i] == ptr2[1000 * j]) 
						{
							w = write(p_1to2, ptr2 +  1000 * j, sizeof(int) * 1000);									
						}
						if(sigCheck2[1][i] == ptr2[1000 * j])
						{
							w = write(p_1to3, ptr2 +  1000 * j, sizeof(int)* 1000);								
						}  
						if(sigCheck2[2][i] == ptr2[1000 * j]) 
						{
							w = write(p_1to4, ptr2 +  1000 * j, sizeof(int)* 1000);
						}
					}
				}			
               	//자기거 체크
				for(i = 0; i < 16; i++)
                {
					for(j = 0; j < 16; j++)
					{
						if(need2[i] == ptr2[1000 * j]) 
						{
							memcpy(ptr2_t +  1000 * i, ptr2 + 1000 * j, sizeof(int) * 1000);
							count_need++;
						}
					}
					printf("%d\n",count_need);
                }
				 //각 client에서 데이터 모으기 
              	while(1)
                {
                    if(b = read(p_2to1, temp1, sizeof(int)* 1000 ) > 0) 
					{						
						for(i = 0; i < 16;i++)
						{
							if(need2[i] == temp1[0]) memcpy(ptr2_t + 1000 * i, temp1, sizeof(int) * 1000);
						}
						count_read++;
					}
					//printf("%d\n",b);
					if(read(p_3to1, temp1, sizeof(int)* 1000 ) > 0)
					{						
						for(i = 0; i < 16;i++)
						{
							if(need2[i] == temp1[0]) memcpy(ptr2_t +  1000 * i, temp1, sizeof(int) * 1000);
						}
						count_read++;
					}
					if(read(p_4to1, temp1, sizeof(int)* 1000 ) > 0) 
					{					
						for(i = 0; i < 16;i++)
						{
							if(need2[i] == temp1[0]) memcpy(ptr2_t +  1000 * i, temp1, sizeof(int) * 1000);
						}
						count_read++;
					}					
					if((16-count_need) == count_read) break;		
					//printf("%d : %d \n",count_need,count_read);
                }      
                //정렬
                for(i = 0; i < 16000; i++)
                {
                   w = ptr2_t[i] / 10;
                   ptr2_s[w] = ptr2_t[i];
                }                
       
                //각 필요한 io로 전송
				for(i = 0 ; i < 8; i++)
				{
                w = write(io0, ptr2_s + i*2 * 1000, sizeof(int)* 1000);				
				w = write(io1, ptr2_s + (i*2+1)  * 1000, sizeof(int)* 1000);				
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
				fd1 = open("computenode2_256_c.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
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
		
			  //블록사이즈로 나눠서 저장
				for (i = 0; i < 16000 ;i++)
				{
					lseek(fd, sizeof(int)*4*i + sizeof(int) * 1, SEEK_SET);
					
					read(fd, ptr2 + i, sizeof(int));
					//memcpy(ptr1[i], rfd1,sizeof(int) * bufsiz);			               
				}
                write(fd1,ptr2,(sizeof(int) * 16000));	
				//필요한 정보 넣기 2
				for (i = 0; i < 4; i++)
				{
					for(j = 0; j< 4; j++)
					need2[i *4 +j] = j * 10 + i * 40000 + 160000;
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
						if(sigCheck2[0][i] == ptr2[1000 * j]) 
						{
							w = write(p_2to1, ptr2 +  1000 * j, sizeof(int) * 1000);									
						}
						if(sigCheck2[1][i] == ptr2[1000 * j])
						{
							w = write(p_2to3, ptr2 +  1000 * j, sizeof(int)* 1000);								
						}  
						if(sigCheck2[2][i] == ptr2[1000 * j]) 
						{
							w = write(p_2to4, ptr2 +  1000 * j, sizeof(int)* 1000);
						}
					}
				}			
               	//자기거 체크
				for(i = 0; i < 16; i++)
                {
					for(j = 0; j < 16; j++)
					{
						if(need2[i] == ptr2[1000 * j]) 
						{
							memcpy(ptr2_t +  1000 * i, ptr2 +  1000 * j, sizeof(int) * 1000);
							count_need++;
						}
					}
                }
				 //각 client에서 데이터 모으기 
              	while(1)
                {
                    if(b = read(p_1to2, temp1, sizeof(int)* 1000 ) > 0) 
					{						
						for(i = 0; i < 16;i++)
						{
							if(need2[i] == temp1[0]) memcpy(ptr2_t + 1000 * i, temp1, sizeof(int) * 1000);
							printf("t %d : %d  \n",need2[i],temp1[0]);	
						}
						
						count_read++;
					}
					//printf("%d\n",b);
					if(read(p_3to2, temp1, sizeof(int)* 1000 ) > 0)
					{						
						for(i = 0; i < 16;i++)
						{
							if(need2[i] == temp1[0]) memcpy(ptr2_t + 1000 * i, temp1, sizeof(int) * 1000);
							printf("t %d : %d  \n",need2[i],temp1[0]);	
						}
						
						count_read++;
					}
					if(read(p_4to2, temp1, sizeof(int)* 1000 ) > 0) 
					{					
						for(i = 0; i < 16;i++)
						{
							if(need2[i] == temp1[0]) memcpy(ptr2_t + 1000 * i, temp1, sizeof(int) * 1000);
							printf("t %d : %d  \n",need2[i],temp1[0]);	
						}
						
						count_read++;
					}			
					
					if((16-count_need) == count_read) break;					
                }      
                //정렬
                for(i = 0; i < 16000; i++)
                {
                   w = ptr2_t[i] / 10 - 16000;
                   ptr2_s[w] = ptr2_t[i];
				  // printf("%d \n", ptr2_s[i]);
                }                
       
               //각 필요한 io로 전송
				for(i = 0 ; i < 8; i++)
				{
                w = write(io0, ptr2_s + i*2 * 1000, sizeof(int)* 1000);
				w = write(io1, ptr2_s + (i*2+1) * 1000, sizeof(int)* 1000);			
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
				fd1 = open("computenode3_256_c.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
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
				
				  //블록사이즈로 나눠서 저장
				for (i = 0; i < 16000 ;i++)
				{
					lseek(fd, sizeof(int)*4*i + sizeof(int) * 2, SEEK_SET);
					
					read(fd, ptr2 +  i, sizeof(int));
					//memcpy(ptr1[i], rfd1,sizeof(int) * bufsiz);			               
				}
                write(fd1,ptr2,(sizeof(int) * 16000));	
				//필요한 정보 넣기 3
					for (i = 0; i < 4; i++)
				{
					for(j = 0; j< 4; j++)
					need2[i *4 +j] = j * 10 + i * 40000 + 320000;
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
						if(sigCheck2[0][i] == ptr2[1000 * j]) 
						{
							w = write(p_3to1, ptr2 + 1000 * j, sizeof(int) * 1000);									
						}
						if(sigCheck2[1][i] == ptr2[1000 * j])
						{
							w = write(p_3to2, ptr2 +  1000 * j, sizeof(int)* 1000);								
						}  
						if(sigCheck2[2][i] == ptr2[1000 * j]) 
						{
							w = write(p_3to4, ptr2 +  1000 * j, sizeof(int)* 1000);
						}
					}
				}			
               	//자기거 체크
				for(i = 0; i < 16; i++)
                {
					for(j = 0; j < 16; j++)
					{
						if(need2[i] == ptr2[1000 * j]) 
						{
							memcpy(ptr2_t +  1000 * i, ptr2 +  1000 * j, sizeof(int) * 1000);
							count_need++;
						}
					}
                }
				 //각 client에서 데이터 모으기 
              	while(1)
                {
                    if(b = read(p_1to3, temp1, sizeof(int)* 1000 ) > 0) 
					{						
						for(i = 0; i < 16;i++)
						{
							if(need2[i] == temp1[0]) memcpy(ptr2_t +  1000 * i, temp1, sizeof(int) * 1000);
						}
						count_read++;
					}
					//printf("%d\n",b);
					if(read(p_2to3, temp1, sizeof(int)* 1000 ) > 0)
					{						
						for(i = 0; i < 16;i++)
						{
							if(need2[i] == temp1[0]) memcpy(ptr2_t +  1000 * i, temp1, sizeof(int) * 1000);
						}
						count_read++;
					}
					if(read(p_4to3, temp1, sizeof(int)* 1000 ) > 0) 
					{					
						for(i = 0; i < 16;i++)
						{
							if(need2[i] == temp1[0]) memcpy(ptr2_t +  1000 * i, temp1, sizeof(int) * 1000);
						}
						count_read++;
					}					
					if((16-count_need) == count_read) break;					
                }      
                //정렬
                for(i = 0; i < 16000; i++)
                {
                   w = ptr2_t[i] / 10 - 32000;
                   ptr2_s[w] = ptr2_t[i];
                }                
       
                //각 필요한 io로 전송
				for(i = 0 ; i < 8; i++)
				{
                w = write(io0, ptr2_s + i*2 *  1000, sizeof(int)* 1000);
				printf("1!!; : %d\n", w);
				w = write(io1, ptr2_s + (i*2+1)  * 1000, sizeof(int)* 1000);
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
				fd1 = open("computenode4_256_c.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);

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
			
				  //블록사이즈로 나눠서 저장
				for (i = 0; i < 16000 ;i++)
				{
					lseek(fd, sizeof(int)*4*i + sizeof(int) * 3, SEEK_SET);
					
					read(fd, ptr2 +  i, sizeof(int));
					//memcpy(ptr1[i], rfd1,sizeof(int) * bufsiz);			               
				}
                write(fd1,ptr2,(sizeof(int) * 16000));	
				//필요한 정보 넣기 4
					for (i = 0; i < 4; i++)
				{
					for(j = 0; j< 4; j++)
					need2[i *4 +j] = j * 10 + i * 40000 + 480000;
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
						if(sigCheck2[0][i] == ptr2[1000 * j]) 
						{
							w = write(p_4to1, ptr2 + 1000 * j, sizeof(int) * 1000);									
						}
						if(sigCheck2[1][i] == ptr2[1000 * j])
						{
							w = write(p_4to2, ptr2 + 1000 * j, sizeof(int)* 1000);								
						}  
						if(sigCheck2[2][i] == ptr2[1000 * j]) 
						{
							w = write(p_4to3, ptr2 + 1000 * j, sizeof(int)* 1000);
						}
					}
				}			
               	//자기거 체크
				for(i = 0; i < 16; i++)
                {
					for(j = 0; j < 16; j++)
					{
						if(need2[i] == ptr2[1000 * j]) 
						{
							memcpy(ptr2_t + 1000 * i, ptr2 +  1000 * j, sizeof(int) * 1000);
							count_need++;
						}
					}
                }
				 //각 client에서 데이터 모으기 
              	while(1)
                {
                    if(b = read(p_1to4, temp1, sizeof(int)* 1000 ) > 0) 
					{						
						for(i = 0; i < 16;i++)
						{
							if(need2[i] == temp1[0]) memcpy(ptr2_t +  1000 * i, temp1, sizeof(int) * 1000);
						}
						count_read++;
					}
					//printf("%d\n",b);
					if(read(p_2to4, temp1, sizeof(int)* 1000 ) > 0)
					{						
						for(i = 0; i < 16;i++)
						{
							if(need2[i] == temp1[0]) memcpy(ptr2_t + 1000 * i, temp1, sizeof(int) * 1000);
						}
						count_read++;
					}
					if(read(p_3to4, temp1, sizeof(int)* 1000 ) > 0) 
					{					
						for(i = 0; i < 16;i++)
						{
							if(need2[i] == temp1[0]) memcpy(ptr2_t +  1000 * i, temp1, sizeof(int) * 1000);
						}
						count_read++;
					}					
					if((16-count_need) == count_read) break;					
                }      
                //정렬
                for(i = 0; i < 16000; i++)
                {
                   w = ptr2_t[i] / 10 - 48000;
                   ptr2_s[w] = ptr2_t[i];
                }                
       
                //각 필요한 io로 전송
				for(i = 0 ; i < 8; i++)
				{
                w = write(io0, ptr2_s + i*2 *  1000, sizeof(int)* 1000);				
				w = write(io1, ptr2_s + (i*2+1) *  1000, sizeof(int)* 1000);			
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