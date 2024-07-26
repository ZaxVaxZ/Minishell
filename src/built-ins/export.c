/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehammoud <ehammoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 21:57:20 by marvin            #+#    #+#             */
/*   Updated: 2024/07/26 19:08:35 by ehammoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	errlog(t_main *m, char *b4, char *var, char *af)
{
	if (write(2, b4, ft_strlen(b4)) == -1)
		free_and_exit(m, ERR_WRT);
	if (write(2, var, ft_strlen(var)) == -1)
		free_and_exit(m, ERR_WRT);
	if (write(2, af, ft_strlen(af)) == -1)
		free_and_exit(m, ERR_WRT);
}

t_bool	export_cmd(t_main *m, char **params)
{
	int		i;
	t_bool	ret;

	if (!params)
		return (True);
	if (!*params)
	{
		print_sorted_env(m->env);
		return (True);
	}
	ret = True;
	i = -1;
	while (params[++i])
	{
		if (!valid_identifier(params[i]))
		{
			ret = False;
			errlog(m, "export: `", params[i], "`: not a valid identifier\n");
		}
		else
			export_var(&m->env, params[i]);
	}
	return (ret);
}
