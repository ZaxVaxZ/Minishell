/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 13:31:22 by pipolint          #+#    #+#             */
/*   Updated: 2024/03/15 16:53:25 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static int	is_directory(char *dir)
{
	struct stat	s;

	if (!stat(dir, &s))
	{
		if (!S_ISDIR(s.st_mode))
			return (1);
		else
			return (0);
	}
	else
	{
		perror("stat()");
		return (-1);
	}
}

int	cd(char *pwd, char *dir)
{
	int			is_dir;
	char		*tmp;

	if (!pwd || !*pwd || !dir || !*dir)
	{
		if (printf("Error in getting current working directory\n") == -1)
			return (-2);
		return (0);
	}
	if (dir[0] != '\\' && dir[0] != '/')
	{
		tmp = ft_strjoin_chr(pwd, '/', dir);
		if (!tmp)
			return (-1);
		is_dir = is_directory(tmp);
		if (is_dir)
			return (is_dir);
	}
	is_dir = is_directory(tmp);
	if (is_dir == 1)
		return (2);
	return (is_dir);
}
