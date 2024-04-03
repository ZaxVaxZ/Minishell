#include <unistd.h>
#include <stdio.h>
#include "libft.h"
int main()
{
	printf("%s\n", getcwd(NULL, 0));
	chdir(ft_strjoin_chr(getcwd(NULL, 0), '/', ".."));
	printf("%s\n", getcwd(NULL, 0));
}