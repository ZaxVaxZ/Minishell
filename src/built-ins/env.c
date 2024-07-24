/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 23:39:56 by marvin            #+#    #+#             */
/*   Updated: 2024/07/24 21:37:59 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

t_bool	print_env(t_main *m)
{
	t_env	*env;

	env = m->env;
	while (env)
	{
		if (env->exported && ft_printf("%s=%s\n", env->key, env->value) == -1)
			free_and_exit(m, ERR_CLS);
		env = env->next;
	}
	return (True);
}
