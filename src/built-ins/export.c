/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 21:57:20 by marvin            #+#    #+#             */
/*   Updated: 2024/07/25 09:53:11 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

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
			if (ft_printf("export: `%s`: not a valid identifier\n",
					params[i]) == -1)
				free_and_exit(m, ERR_WRT);
		}
		else
			export_var(&m->env, params[i]);
	}
	return (ret);
}
