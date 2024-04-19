#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	char	*line;
	char	*cwd;

	cwd = getcwd(NULL, -1);
	line = readline(cwd);
	while (line)
	{
		add_history(line);
		free(line);
		write(1, "Line: ", 6);
		write(1, line, strlen(line));
		write(1, "\n", 1);
		line = readline(">");
	}
}