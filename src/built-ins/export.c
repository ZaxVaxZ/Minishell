/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 21:57:20 by marvin            #+#    #+#             */
/*   Updated: 2024/04/02 15:51:55 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

t_bool	export_cmd(t_env **env, char **variable)
{
	int		i;
	t_env	*clone;

	i = 0;
	if (!*variable)
	{
		print_sorted_env(*env);
		return (True);
	}
	while (variable[i])
	{
		if (!export_var(env, variable[i]))
			return (free_env(env));
		i++;
	}
	return (True);
}
