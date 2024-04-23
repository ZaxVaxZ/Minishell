/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 13:06:15 by pipolint          #+#    #+#             */
/*   Updated: 2024/03/13 20:52:17 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/*
	set the old pwd as the current pwd
	use chdir to change directories
*/
void	cd(t_builtins *b, char *dir)
{
	b->old_pwd = b->pwd;
	printf("Directory passed: %s\n", dir);
	if (chdir(dir) == -1)
		perror(NULL);
	b->pwd = getenv("PWD");
	printf("Old PWD: %s\n", b->old_pwd);
	printf("Current PWD: %s\n", b->pwd);
}

t_bool	echo(char *str, t_bool n)
{
	if (write(1, str, ft_strlen(str)) == -1)
		return (False);
	if (n && write(1, "\n", 1) == -1)
		return (False);
	return (True);
}

void	exiting(int code)
{
	exit(WEXITSTATUS(code));
}

void	working_dir(t_builtins *b)
{
	write(1, b->pwd, ft_strlen(b->pwd));
	write(1, "\n", 1);
}
