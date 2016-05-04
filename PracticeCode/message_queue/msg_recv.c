#define PROJECT_ID 0

#include <stdio.h>
#include <sys/msg.h>

#include "my_msg.h"

int main(int argc, char** argv)
{
	key_t key = ftok(u8"/home/dainslef", PROJECT_ID);
	struct my_msg msg;
	int msg_id = 0;

	if ((msg_id = msgget(key, 0)) == -1)
		perror("msgget");

	while (1)
	{
		int size = 0;
		if ((size = msgrcv(msg_id, &msg, sizeof(msg.data), -300, IPC_NOWAIT)) == -1)
		{
			perror("msgrcv");
			if (msgctl(msg_id, IPC_RMID, NULL) == -1)
				perror("msgctl");
			break;
		}
		else
			printf("Num: %d\nMessage type: %ld\nRecevie: %s\n\n", msg.data.num, msg.type, msg.data.text);
	}

	return 0;
}
