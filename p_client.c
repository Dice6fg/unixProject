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
	int rfd2[2000];
	int ptr1[4][1000];
    int ptr1_t[4][1000];
	int ptr2[8][2000];
    int ptr2_t[8][2000];
    int p_1to2, p_2to1, p_3to4, p_4to3;
	int io0, io1;
	char* fifoname;
	int bufsiz;

	int w = 0;
    int b = 0;
	
    int count = 0;

    if (atoi(argv[2]) == 64) 
	{
		bufsiz = 1000;	
		fd = open("data1.txt", O_RDONLY);

		switch (atoi(argv[1]))
		{
			case 0: 
				fd1 = open("computenode1_64.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
				io0 = open("fifo0", O_WRONLY);
			
                p_2to1 = open("2to1",O_RDONLY | O_NONBLOCK);
                p_1to2 = open("1to2",O_WRONLY);
              
                //블록사이즈로 나눠서 저장
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
                //각 client에 데이터 전송 1
                for(i = 0; i < 4;i++)
				{
                    for(j = 0; j < bufsiz / 2; j++ )
					{
                        w = write(p_1to2, &ptr1[i][(j*2) +1], sizeof(int));
                      
					}
              
				}
                //각 client에서 데이터 모으기 1
                for(i = 0; i < 4; i++)
                {
                    count = 0;
                    while(1)
                    {
                        if(b = read(p_2to1, &ptr1_t[i][count], sizeof(int)) > 0) count++;              
                        if(count == bufsiz/2) break;
                    }
                }

                for(i = 0; i < 4;i++)
				{
                    for(j = bufsiz/2 -1; j > -1 ; j-- )
					{
                        ptr1_t[i][bufsiz/2 + j] = ptr1_t[i][j];                     
					}
                    for(j = bufsiz/2 -1; j > -1 ; j-- )
					{
                        ptr1_t[i][j] = ptr1[i][(j*2)]; 
                        
					}
				}
                //io0로 전송
                for(i = 0; i < 4;i++)
				{
					for(j = 0; j < bufsiz; j++)
					{
						w = write(io0, &ptr1_t[i][j], sizeof(int));
						
					}
				}
                close(p_2to1);
                close(p_1to2);
				close(fd1);
				close(io0);
				break;
			case 1: 
				fd1 = open("computenode2_64.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
				io1 = open("fifo5", O_WRONLY);
                p_2to1 = open("2to1",O_WRONLY);
                p_1to2 = open("1to2",O_RDONLY| O_NONBLOCK);
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
                //각 client에 데이터 전송 2
                for(i = 0; i < 4;i++)
				{
                    for(j = 0; j < bufsiz / 2; j++ )
					{
                        write(p_2to1, &ptr1[i][(j*2)], sizeof(int));
                       
					}
				}
                //각 client에서 데이터 모으기 2
                for(i = 0; i < 4; i++)
                {
                    count = 0;
                    while(1)
                    {
                        if(b = read(p_1to2, &ptr1_t[i][count], sizeof(int)) > 0) count++;

                        if(count == bufsiz/2)
                        break;
                    }
                }

                for(i = 0; i < 4;i++)
				{
                    for(j = bufsiz/2 -1; j > -1 ; j-- )
					{
                        ptr1_t[i][j] = ptr1_t[i][j];
                    }
                    for(j = bufsiz/2 -1; j > -1 ; j-- )
					{                        
                        ptr1_t[i][bufsiz / 2 +j] = ptr1[i][(j*2) + 1];                         
                    }
				}
				//io1로 전송
                for(i = 0; i < 4;i++)
				{
					for(j = 0; j < bufsiz; j++)
					{
						w = write(io1, &ptr1_t[i][j], sizeof(int));
					
					}
				}

				close(p_2to1);
                close(p_1to2);
				close(fd1);
				close(io1);
				break;
			case 2:
				fd1 = open("computenode3_64.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
				io0 = open("fifo2", O_WRONLY);
                p_3to4 = open("3to4",O_WRONLY);
                p_4to3 = open("4to3",O_RDONLY| O_NONBLOCK);
				
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

                //각 client에 데이터 전송 3                
                for(i = 0; i < 4;i++)
				{
                    for(j = 0; j < bufsiz / 2; j++ )
					{
                        write(p_3to4, &ptr1[i][(j*2) +1], sizeof(int));
                      
					}
				}
                //각 client에서 데이터 모으기 3
                for(i = 0; i < 4; i++)
                {
                    count = 0;
                    while(1)
                    {
                        if(b = read(p_4to3, &ptr1_t[i][count], sizeof(int)) > 0) count++;

                        if(count == bufsiz/2)
                        break;
                    }
                }

                for(i = 0; i < 4;i++)
				{
                    for(j = bufsiz/2 -1; j > -1 ; j-- )
					{
                        ptr1_t[i][bufsiz/2 + j] = ptr1_t[i][j];                     
					}
                    for(j = bufsiz/2 -1; j > -1 ; j-- )
					{
                        ptr1_t[i][j] = ptr1[i][(j*2)]; 
                        
					}
				}
                //io1로 전송
                for(i = 0; i < 4;i++)
				{
					for(j = 0; j < bufsiz; j++)
					{
						w = write(io0, &ptr1_t[i][j], sizeof(int));
						
					}
				}

				close(p_3to4);
                close(p_4to3);
				close(fd1);
				close(io0);
				break;
			case 3:
				io1 = open("fifo7", O_WRONLY);
				fd1 = open("computenode4_64.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
                p_3to4 = open("3to4",O_RDONLY| O_NONBLOCK);
                p_4to3 = open("4to3",O_WRONLY);
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
                 //각 client에 데이터 전송 4
                for(i = 0; i < 4;i++)
				{
                    for(j = 0; j < bufsiz / 2; j++ )
					{
                        write(p_4to3, &ptr1[i][(j*2)], sizeof(int));
                      
					}
				}
                //각 client에서 데이터 모으기 4
                
                for(i = 0; i < 4; i++)
                {
                    count = 0;
                    while(1)
                    {                   
                    
                        if(b = read(p_3to4, &ptr1_t[i][count], sizeof(int)) > 0) count++;

                        if(count == bufsiz/2)
                        break;
                    }
                }

                for(i = 0; i < 4;i++)
				{
                     for(j = bufsiz/2 -1; j > -1 ; j-- )
					{
                        ptr1_t[i][j] = ptr1_t[i][j];
                    }
                    for(j = bufsiz/2 -1; j > -1 ; j-- )
					{                        
                        ptr1_t[i][bufsiz / 2 +j] = ptr1[i][(j*2) + 1];                         
                    }
				}	
                //io1로 전송
                for(i = 0; i < 4;i++)
				{
					for(j = 0; j < bufsiz; j++)
					{
						w = write(io1, &ptr1[i][j], sizeof(int));
					
					}
				}

				close(p_3to4);
                close(p_4to3);
				close(fd1);
				close(io1);
				break;

			default: break;
		}
		close(fd);
	}
	else if(atoi(argv[2]) == 256) 
	{
		bufsiz = 2000;		
		fd = open("data2.txt", O_RDONLY);

		switch (atoi(argv[1]))
		{
		case 0:
			io0 = open("fifo0", O_WRONLY);			
			fd1 = open("computenode1_256.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
            p_2to1 = open("2to1",O_RDONLY| O_NONBLOCK);
            p_1to2 = open("1to2",O_WRONLY);
			for (i = 0; i < 8; i++)
			{
				lseek(fd, sizeof(int) * bufsiz * (2*i), SEEK_SET);
				read(fd, rfd2, sizeof(rfd2));
				for (j = 0; j < bufsiz; j++)
				{
					ptr2[i][j] = rfd2[j];
				}
				write(fd1, rfd2, sizeof(rfd2));
			
			}
            
            //각 client에 데이터 전송 1
               for(i = 0; i < 8;i++)
				{
                    for(j = 0; j < bufsiz / 2; j++ )
					{
                        write(p_1to2, &ptr2[i][(j*2) +1], sizeof(int));
                         
					}
				}
            
                 //각 client에서 데이터 모으기 1
               
                for(i = 0; i < 8; i++)
                {
                    count = 0;
                    while(1)
                    {
                        if(b = read(p_2to1, &ptr2_t[i][count], sizeof(int)) > 0) count++;

                        if(count == bufsiz/2)
                        break;
                    }
                }

                for(i = 0; i < 8;i++)
				{
                    for(j = bufsiz/2 -1; j > -1 ; j-- )
					{
                        ptr2_t[i][bufsiz/2 + j] = ptr2_t[i][j];                     
					}
                    for(j = bufsiz/2 -1; j > -1 ; j-- )
					{
                        ptr2_t[i][j] = ptr2[i][(j*2)]; 
                        
					}
				}
                //io0로 전송
                for(i = 0; i < 8;i++)
				{
					for(j = 0; j < bufsiz; j++)
					{
						w = write(io0, &ptr2_t[i][j], sizeof(int));
						
					}
				}
			
                       
            close(p_2to1);
            close(p_1to2);
			close(fd1);
			close(io0);
			break;
		case 1:
			io1 = open("fifo5", O_WRONLY);
			fd1 = open("computenode2_256.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
            p_2to1 = open("2to1",O_WRONLY);
            p_1to2 = open("1to2",O_RDONLY| O_NONBLOCK);
			for (i = 0; i < 8; i++)
			{
				lseek(fd, sizeof(int) * bufsiz * ((2*i) + 1), SEEK_SET);
				read(fd, rfd2, sizeof(rfd2));
				for (j = 0; j < bufsiz; j++)
				{
					ptr2[i][j] = rfd2[j];
				}
				write(fd1, rfd2, sizeof(rfd2));

			}

			//각 client에 데이터 전송 2
                for(i = 0; i < 8;i++)
				{
                    for(j = 0; j < bufsiz / 2; j++ )
					{
                        write(p_2to1, &ptr2[i][(j*2)], sizeof(int));
                        
					}
				}

				//각 client에서 데이터 모으기 2
                for(i = 0; i < 8; i++)
                {
                    count = 0;
                    while(1)
                    {
                        if(b = read(p_1to2, &ptr2_t[i][count], sizeof(int)) > 0) count++;

                        if(count == bufsiz/2)
                        break;
                    }
                }

                for(i = 0; i < 8;i++)
				{
                    for(j = bufsiz/2 -1; j > -1 ; j-- )
					{
                        ptr2_t[i][j] = ptr2_t[i][j];
                    }
                    for(j = bufsiz/2 -1; j > -1 ; j-- )
					{                        
                        ptr2_t[i][bufsiz / 2 +j] = ptr2[i][(j*2) + 1];                         
                    }
				}

                 //io1로 전송
                for(i = 0; i < 8;i++)
				{
					for(j = 0; j < bufsiz; j++)
					{
						w = write(io1, &ptr2_t[i][j], sizeof(int));
						
					}
				}
			
			 close(p_2to1);
                close(p_1to2);
				close(fd1);
				close(io1);
			break;
		case 2:
			io0 = open("fifo2", O_WRONLY);
			fd1 = open("computenode3_256.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
            p_3to4 = open("3to4",O_WRONLY);
            p_4to3 = open("4to3",O_RDONLY| O_NONBLOCK);
			for (i = 0; i < 8; i++)
			{
				lseek(fd, sizeof(int) * bufsiz * ((2*i) + 16), SEEK_SET);
				read(fd, rfd2, sizeof(rfd2));
				for (j = 0; j < bufsiz; j++)
				{
					ptr2[i][j] = rfd2[j];
				}
				write(fd1, rfd2, sizeof(rfd2));

			}
			
			//각 client에 데이터 전송 3
                for(i = 0; i < 8;i++)
				{
                    for(j = 0; j < bufsiz / 2; j++ )
					{
                        write(p_3to4, &ptr2[i][(j*2) +1], sizeof(int));
                        
					}
				}
				 //각 client에서 데이터 모으기 3
                 for(i = 0; i < 8; i++)
                {
                    count = 0;
                    while(1)
                    {
                        if(b = read(p_4to3, &ptr2_t[i][count], sizeof(int)) > 0) count++;

                        if(count == bufsiz/2)
                        break;
                    }
                }

                 for(i = 0; i < 8;i++)
				{
                    for(j = bufsiz/2 -1; j > -1 ; j-- )
					{
                        ptr2_t[i][bufsiz/2 + j] = ptr2_t[i][j];                     
					}
                    for(j = bufsiz/2 -1; j > -1 ; j-- )
					{
                        ptr2_t[i][j] = ptr2[i][(j*2)]; 
                        
					}
				}

                 //io1로 전송
                for(i = 0; i < 8;i++)
				{
					for(j = 0; j < bufsiz; j++)
					{
						w = write(io0, &ptr2_t[i][j], sizeof(int));
					
					}
				}
			
			 close(p_3to4);
                close(p_4to3);
				close(fd1);
				close(io0);
			break;
		case 3:
			io1 = open("fifo7", O_WRONLY);
			fd1 = open("computenode4_256.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
            p_3to4 = open("3to4",O_RDONLY| O_NONBLOCK);
            p_4to3 = open("4to3",O_WRONLY);
			for (i = 0; i < 8; i++)
			{
				lseek(fd, sizeof(int) * bufsiz * ((2*i) + 17), SEEK_SET);
				read(fd, rfd2, sizeof(rfd2));
				for (j = 0; j < bufsiz; j++)
				{
					ptr2[i][j] = rfd2[j];
				}
				write(fd1, rfd2, sizeof(rfd2));

			}

			//각 client에 데이터 전송 4
                for(i = 0; i < 8;i++)
				{
                    for(j = 0; j < bufsiz / 2; j++ )
					{
                        write(p_4to3, &ptr2[i][(j*2)], sizeof(int));
                       
					}
				}

				//각 client에서 데이터 모으기 4
                for(i = 0; i < 8; i++)
                {
                    count = 0;
                    while(1)
                    {
                        if(b = read(p_3to4, &ptr2_t[i][count], sizeof(int)) > 0) count++;

                        if(count == bufsiz/2)
                        break;
                    }
                }

                 for(i = 0; i < 8;i++)
				{
                     for(j = bufsiz/2 -1; j > -1 ; j-- )
					{
                        ptr2_t[i][j] = ptr2_t[i][j];
                    }
                    for(j = bufsiz/2 -1; j > -1 ; j-- )
					{                        
                        ptr2_t[i][bufsiz / 2 +j] = ptr2[i][(j*2) + 1];                         
                    }
				}	

                  //io1로 전송
                for(i = 0; i < 8;i++)
				{
					for(j = 0; j < bufsiz; j++)
					{
						w = write(io1, &ptr2[i][j], sizeof(int));
					
					}
				}


			close(p_3to4);
                close(p_4to3);
				close(fd1);
				close(io1);
			break;
		default: break;
		}
		close(fd);
	}
}