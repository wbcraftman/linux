#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<fcntl.h>
#include<string.h>
#include<sys/wait.h>

int main()
{
	pid_t pid;
	pid_t waitPid;
	int i,fd;
	int j = 0;
	char* buff = "it is a test for daemon.\n";
	
	pid = fork();//创建子进程
	
	if(pid == -1)
	{
		printf("creating child process error!\n");
		exit(1);
	}
	else if(pid == 0)
	{
		printf("this is child process.\n");
		sleep(3);
		
		setsid();//建立会话组
		chdir("/");//将当前目录修改为根目录
		umask(0);//重置文件权限掩码
		for(i = 0; i < getdtablesize();i++)
		{
			close(i);//关闭文件描述符
		}
		
		/*守护进程开始运行*/
		while(j < 10)
		{
			if((fd = open("/home/wb/Documents/daemon.log", O_CREAT|O_WRONLY|O_APPEND, 0600)) < 0)
			{
				printf("open this file is error.\n");
				exit(1);
			}
			
			write(fd, buff, strlen(buff)+1);
			close(fd);
			sleep(10);
			j++;
		}
		exit(0);
	}
	else
	{
		exit(0);//父进程退出
	}
	return 0;
}
