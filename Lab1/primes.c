#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


void prime(int fd) {
	int num;
	int status;
	if (!read(fd, &num, sizeof(num))) { // if read nothing, pipeline is empty, not a prime
		exit(0);
	}
	// is a prime
	printf("prime %d\n", num);

	int p[2];
	pipe(p);
	if (fork() == 0) {
		// child
		close(p[1]);
		prime(p[0]);
	}
	else {
		// parent
		close(p[0]);
		int n;
		int eof;
		do {
			eof = read(fd, &n, sizeof(n));
			if (n%num != 0) { // prime
				write(p[1], &n, sizeof(n));
			}
		} while (eof);
		close(p[1]);
	}
	wait(&status);
	exit(0);
}
	

int
main(int argc, char* argv[])
{
	int parent[2];
	int status;
	pipe(parent);
	if (fork() == 0) {
		//child
		// no write parent write port
		close(parent[1]);
		prime(parent[0]); // pass read port
	}
	else {
		// parent
		close(parent[0]);
		int i;
		for (i=2; i<36; ++i) {
			// no need to read
			write(parent[1], &i, sizeof(i));
		}
		close(parent[1]);
	}
	wait(&status);
	exit(0);
}


