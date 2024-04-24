#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
	char *str = "hello there";
	for (int i = 0; i < 12; i++)
	{
		//if (i % 2)
		//	str[i] = '\b';
		//else
			//str[i] = [i];
		write(1, str + i, 1);
		usleep(60000);
	}
	for (int i = 0; i < 12; i++)
	{
		//if (i % 2)
		//	str[i] = '\b';
		//else
			//str[i] = [i];
		write(1, "\b", 1);
		usleep(60000);
	}
}
