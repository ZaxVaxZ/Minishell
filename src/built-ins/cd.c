/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 13:31:22 by pipolint          #+#    #+#             */
/*   Updated: 2024/07/24 21:24:12 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

t_bool	cd_home(t_main *m, char *pwd)
{
	if (get_var(m->env, "HOME"))
	{
		if (set_var(&m->env, "OLDPWD", pwd, True) == False)
			free_and_exit(m, ERR_CLS);
		chdir(get_var(m->env, "HOME"));
		if (set_var(&m->env, "PWD", get_var(m->env, "HOME"), True) == False)
			free_and_exit(m, ERR_CLS);
	}
	else
	{
		if (write(2, "cd: HOME not set\n", 17) == -1)
			free_and_exit(m, ERR_WRT);
		return (False);
	}
	return (True);
}

t_bool	cd(t_main *m, char *pwd, char *dir)
{
	char	*tmp;
	t_bool	ret;

	if (!dir)
		return (cd_home(m, pwd));
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
	if (set_var(&m->env, "OLDPWD", pwd, True) == False)
		return (False);
	tmp = getcwd(NULL, 0);
	if (!tmp)
		return (False);
	ret = set_var(&m->env, "PWD", tmp, True);
	free(tmp);
	return (ret);
}
