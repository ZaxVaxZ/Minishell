/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 23:39:56 by marvin            #+#    #+#             */
/*   Updated: 2024/03/31 13:27:13 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

t_bool	print_env(t_env *env)
{
	while (env)
	{
		if (env->exported && ft_printf("%s=%s\n", env->key, env->value) == -1)
			return (False);
		env = env->next;
	}
	return (True);
}
