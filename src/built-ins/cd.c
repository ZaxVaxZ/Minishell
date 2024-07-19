/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 13:31:22 by pipolint          #+#    #+#             */
/*   Updated: 2024/07/17 09:42:49 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

t_bool	cd_home(t_env **env, char *pwd)
{
	if (get_var(*env, "HOME"))
	{
		if (set_var(env, "OLDPWD", pwd, True) == False)
			return (False);
		chdir(get_var(*env, "HOME"));
		if (set_var(env, "PWD", get_var(*env, "HOME"), True) == False)
			return (False);
	}
	else
	{
		write(2, "cd: HOME not set\n", 17);
		return (False);
	}
	return (True);
}

t_bool	cd(t_env **env, char *pwd, char *dir)
{
	char	*tmp;
	t_bool	ret;

	if (!dir)
		return (cd_home(env, pwd));
	if (dir[0] == '/')
		tmp = ft_strdup(dir);
	else
		tmp = ft_strjoin_chr(pwd, '/', dir);
	ret = (chdir(dir) == -1);
	if (tmp)
		free(tmp);
	if (ret)
	{
		perror(dir);
		return (False);
	}
	if (set_var(env, "OLDPWD", pwd, True) == False)
		return (False);
	tmp = getcwd(NULL, 0);
	if (!tmp)
		return (False);
	ret = set_var(env, "PWD", tmp, True);
	free(tmp);
	return (ret);
}
