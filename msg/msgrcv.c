//接收进程
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#define MAX_BUFFER_LEN 1024 

//定义消息的结构体
struct message
{
	long msg_type;
	char msg_text[MAX_BUFFER_LEN];

};

int main()
{
	int msg_id;
	int key;
	struct message msg;
	
	key = ftok(".", 'a');//利用ftok函数将当前目录与'a'共同转化为key_t类型的键值
	if(key == -1)
	{
		perror("creat key");
		exit(1);
	}

	msg_id = msgget(key, IPC_CREAT|0666);//创建消息队列，并设置文件权限掩码0666
	if(msg_id == -1)
	{
		perror("get the message queue");
		exit(1);
	}
	else
	{
		printf("receive message ID is %d\n", msg_id);
	}

	while(1)
	{
		memset(msg.msg_text, 0, MAX_BUFFER_LEN);//将消息队列清空
		if(msgrcv(msg_id, &msg, MAX_BUFFER_LEN, 0, 0) == -1)//接收进程从消息队列读取消息
		{
			perror("receive mess, error");
			exit(1);
		}
		else
		{
			printf("the message is %s\n", msg.msg_text);
		}

		if(strncmp(msg.msg_text, "quit", 4) == 0)//如果接收的消息为quit，则退出接收进程
		{
			break;
		}
	}
	
	if(msgctl(msg_id, IPC_RMID, NULL) < 0)//IPC_RMID为从进程中删除消息队列
	{
		perror("msgctl");
		exit(1);
	}
	exit(0);//接收进程正常结束
	return 0;
}
