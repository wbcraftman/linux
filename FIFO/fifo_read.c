#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

#define MAX_BUFFER_LEN 512
#define LOG_PATH "./fifo" //在当前目录下设置FIFO文件

int main()
{
	int fd, fifo_read;
	char buffer[MAX_BUFFER_LEN];

	if(mkfifo(LOG_PATH, 0666) < 0)
	{
		printf("open fifo.log error.\n");
		exit(1);
	}

	fd = open(LOG_PATH, O_RDONLY);//打开FIFO管道的文件描述符，并设置只读权限
	if(fd < 0)
	{
		printf("open file error.\n");
		exit(1);
	}

	while(1)
	{
		memset(buffer, 0, sizeof(buffer));//清空缓冲区
		fifo_read = read(fd, buffer, MAX_BUFFER_LEN);//从管道中读取数据，如果没有数据则阻塞
		if(fifo_read < 0)
		{
			printf("read file error.\n");
			exit(1);
		}
		printf("%d bytes from the input string: %s.\n", fifo_read, buffer);
		sleep(1);
	}
	exit(0);
	return 0;
}
