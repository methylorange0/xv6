#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, const char *argv[])
{
	int pid;
	int p[2];
	pipe(p);

	if (fork() == 0)
	{
		pid = getpid();
		char buf[1];
		if (read(p[0], buf, 1) != 1)
		{
			fprintf(2, "error: failed to read\n");
			exit(1);
		}
		close(p[0]);
		printf("%d: received ping\n", pid);
		if(write(p[1], buf, 1) != 1)
		{
			fprintf(2, "error: failed to write\n");
			exit(1);
		}
		close(p[1]);
		exit(0);
	}else{
		pid = getpid();
		char byte[1] = "a";
		char buf[1];
		if (write(p[1], byte, 1) != 1)
		{
			fprintf(2, "error: failed to write\n");
			exit(1);
		}
		close(p[1]);
		if(read(p[0], buf, 1) != 1){
			fprintf(2, "error: failed to read\n");
			exit(1);
		}
		printf("%d: received pong\n", pid);
		close(p[0]);
		exit(0);
	}
}