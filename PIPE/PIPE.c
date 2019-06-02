#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>

#define MAX_BUFFER_LEN 512

int main()
{
	pid_t pid;
	int pipe_fd[2], pipe_write,pipe_read;
	const char buffer_pid[] = "it is a test for pipe.\n";
	char buffer[MAX_BUFFER_LEN];

	memset(buffer, 0 ,sizeof(buffer));
	
	/*创建管道*/
	if(pipe(pipe_fd) == -1)
	{
		printf("creating pipe error.\n");
		exit(1);
	}
	else
	{
		printf("creating pipe successfully.\n");
	}

	/*创建子进程*/
	pid = fork();
	if(pid == -1)
	{
		printf("creating child process error.\n");
		exit(1);
	}
	else if(pid == 0)
	{
		printf("this is a child process.\n");
		close(pipe_fd[1]);//子进程关闭“写”通道
		sleep(3);
		if((pipe_read = read(pipe_fd[0], buffer, MAX_BUFFER_LEN)) > 0)//从PIPE中读取数据，如果没收到数组则阻塞等待
		{
			printf("%d bytes read from the pipe is %s. \n", pipe_read, buffer);
			exit(1);
		}
		close(pipe_fd[0]);
		exit(0);//退出子进程
	}
	else
	{
		printf("this is parent process.\n");
		close(pipe_fd[0]);//父进程关闭“读”通道
		sleep(1);
		if(pipe_write = write(pipe_fd[1], buffer_pid, strlen(buffer_pid)) < 0)//父进程向PIPE中写入数据
		{
			printf("write error");
			exit(1);
		}
		close(pipe_fd[1]);
		waitpid(pid,NULL,0);//等待子进程退出，如果未退出则阻塞等待
		exit(0);
	}
	return 0;
}
