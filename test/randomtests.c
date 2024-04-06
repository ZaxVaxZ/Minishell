#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>


int	print_child()
{
	pid_t pid;
	pid = fork();
	printf("Hi %d\n", (int)pid);
	printf("Woah %d\n", (int)pid);
	return ((int)pid);
}

int main()
{
	printf("ONCE %d\n", (int)getpid());
	printf("TWICE %d\n", print_child());
	printf("HUH?\n");
}