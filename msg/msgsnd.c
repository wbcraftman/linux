#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_BUFFER_LEN 1024

struct message
{
	long msg_type;
	char msg_text[MAX_BUFFER_LEN];
};

int main()
{
	int msg_id;
	key_t key;
	struct message msg;
	
	key = ftok(".", 'a');
	if(key == -1)
	{
		perror("ftok");
		exit(1);
	}

	msg_id = msgget(key, IPC_CREAT|0666);
	if(msg_id == -1)
	{
		perror("creat msg");
		exit(1);
	}
	else
	{
		printf("the message queue ID is %d\n", msg_id);
	}
	
	while(1)
	{
		printf("enter some message to the queus:");
		if(fgets(msg.msg_text, MAX_BUFFER_LEN, stdin) == NULL)
		{
			puts("no message");
			exit(1);
		}
		msg.msg_type = getpid();
		if(msgsnd(msg_id, &msg, strlen(msg.msg_text), 0) == -1)
		{
			perror("message posted.\n");
			exit(1);
		}
		if(strncmp(msg.msg_text, "quit", 4) == 0)
		{
			break;
		}
	}

	exit(0);
	return 0;
}
