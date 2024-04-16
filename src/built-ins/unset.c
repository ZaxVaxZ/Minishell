/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 03:56:02 by marvin            #+#    #+#             */
/*   Updated: 2024/04/03 16:19:52 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

t_bool	valid_identifier(char *id)
{
	int		i;
	t_bool	found_eq;

	i = 0;
	if (found_in(id[0], DIGIT) || id[0] == EQUAL)
		return (False);
	found_eq = False;
	while (id[i])
	{
		found_eq = (found_eq || id[i] == EQUAL);
		if (!found_eq && !is_valid_var_char(id[i]))
			return (False);
		i++;
	}
	return (True);
}

t_bool	unset(t_env **env, char **params)
{
	int		i;
	t_bool	ret;

	ret = True;
	if (!params)
		return (True);
	i = 0;
	while (params[i])
	{
		if (!valid_identifier(params[i]))
		{
			ret = False;
			ft_printf("unset: `%s`: not a valid identifier\n", params[i]);
		}
		else
			delete_var(env, params[i]);
		i++;
	}
	return (ret);
}
