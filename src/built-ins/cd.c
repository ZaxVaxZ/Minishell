/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehammoud <ehammoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 13:31:22 by pipolint          #+#    #+#             */
/*   Updated: 2024/04/15 15:20:49 by ehammoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

t_bool	cd(t_env **env, char *pwd, char *dir)
{
	char	*tmp;
	t_bool	ret;

	if (!dir)
		return (True);
	if (dir[0] == '/')
		tmp = ft_strdup(dir);
	else
		tmp = ft_strjoin_chr(pwd, '/', dir);
	ret = (chdir(dir) == -1);
	if (tmp)
		free(tmp);
	if (ret)
		perror(dir);
	if (ret)
		return (True);
	if (set_var(env, "OLDPWD", pwd, True) == False)
		return (False);
	tmp = getcwd(NULL, 0);
	ret = set_var(env, "PWD", tmp, True);
	free(tmp);
	return (ret);
}
