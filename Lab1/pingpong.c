#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
main(int argc, char * argv[])
{
	int parent[2], child[2];
	int status;
	char info[1] = "A";
	pipe(parent);
	pipe(child);

	if (fork() == 0) {
		// child
		close(parent[1]);
		close(child[0]);
		read(parent[0], info, 1);
		printf("%d: received ping\n", getpid());
		write(child[1], info, 1);
		close(parent[0]);
		close(child[1]);
	}
	else {
		close(parent[0]);
		close(child[1]);
		read(parent[1], info, 1);
		printf("%d: received pong\n", getpid());
		write(child[0], info, 1);
		close(parent[1]);
		close(child[0]);
	}
	wait(&status);
	exit(0);
}


