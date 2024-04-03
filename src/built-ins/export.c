/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 21:57:20 by marvin            #+#    #+#             */
/*   Updated: 2024/04/03 16:00:34 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

t_bool	export_cmd(t_env **env, char **params)
{
	int		i;
	t_bool	ret;

	if (!params)
		return (True);
	if (!*params)
	{
		print_sorted_env(*env);
		return (True);
	}
	ret = True;
	i = 0;
	while (params[i])
	{
		if (!valid_identifier(params[i]))
		{
			ret = False;
			ft_printf("export: `%s`: not a valid identifier\n", params[i]);
		}
		else
			export_var(env, params[i]);
		i++;
	}
	return (ret);
}
