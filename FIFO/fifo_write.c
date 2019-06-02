#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>

#define MAX_LENGTH_LEN 512
#define LOG_PATH "./fifo"

int main()
{
	int fd, fifo_write;
	char buffer[] = "it is a test for fifo.\n";

	fd = open(LOG_PATH, O_WRONLY);
	if(fd < 0)
	{
		printf("open file error.\n");
		exit(1);
	}
	
	while(1)
	{
		fifo_write = write(fd, buffer, strlen(buffer));
		if(fifo_write < 0)
		{
			printf("write file error.\n");
			exit(1);
		}
		printf("%d bytes wrtite in the fifo.log: %s.\n", fifo_write,buffer);
		sleep(2);
	}
	exit(0);
	return 0;
}
