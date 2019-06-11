#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<string.h>

#define MAX_BUFFER_LEN 1024

int main()
{
	pid_t pid;
	int shm_id;
	char* shm_addr;
	char flag[] = "WROTE";
	char buff[MAX_BUFFER_LEN];
	int i = 0;

	memset(buff, 0, sizeof(buff));
	shm_id = shmget(IPC_PRIVATE, MAX_BUFFER_LEN, 0666);//创建共享内存文件，并设置文件掩码
	if(shm_id == -1)
	{
		printf("creat share memory fail.\n");
		return 0;
	}
	else
	{
		printf("creat share memeory: %d\n", shm_id);
	}

	system("ipcs -m");

	pid = fork();//创建子进程
	if(pid == -1)
	{
		printf("creat child process fail.\n");
		exit(1);
	}
	else if(pid == 0)//子进程进入该分支
	{
		printf("this is child process.\n");
		shm_addr = shmat(shm_id, 0, 0);
		if(shm_addr == (void*)-1)
		{
			perror("child: shmat");
			exit(1);
		}
		else
		{
			printf("child: attach shared-memory: %p\n", shm_addr);
		}
		system("ipcs -m");

		while((strncmp(shm_addr, flag, strlen(flag))) && (i <= 10))
		{
			printf("child: wait for emable data..\n");
			sleep(5);
			i++;
		}

		strcpy(buff, shm_addr+strlen(flag));
		printf("child: share-memory: %s\n", buff);

		if(shmdt(shm_addr) == (void*)-1)
		{
			perror("shmat");
			exit(1);
		}
		else
		{
			printf("child: deattach shared-memory.\n");
		}

		system("ipcs -m");
		if(shmctl(shm_id, IPC_RMID, NULL) == -1)
		{
			perror("child: shmctl(IPC_RMID)\n");
			exit(1);
		}
		else
		{
			printf("delete shared-memory.\n");
		}
		system("ipcs -m");
		exit(0);
	}
	else//父进程进入该分支
	{
		printf("this is parent process.\n");
		shm_addr = shmat(shm_id, 0, 0);
		if(shm_id == -1)
		{
			perror("creat share memory");
			exit(1);
		}
		else
		{
			printf("parent process share memory address: %p\n", shm_addr);
		}
		sleep(1);
		printf("\ninput some string:\n");
		fgets(buff, MAX_BUFFER_LEN, stdin);
		strncpy(shm_addr+strlen(flag), buff, strlen(buff));
		strncpy(shm_addr, flag, strlen(flag));

		if(shmdt(shm_addr) == -1)
		{
			perror("parent: shmdt");
			exit(1);
		}
		else
		{
			printf("parent: deattach share memory\n");
		}
		system("ipcs -m");

		waitpid(pid, NULL, 0);
		printf("finish\n");
	}
	exit(0);
	return 0;
}
