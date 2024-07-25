/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 03:56:02 by marvin            #+#    #+#             */
/*   Updated: 2024/07/25 09:53:22 by pipolint         ###   ########.fr       */
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

t_bool	unset(t_main *m, char **params)
{
	int		i;
	t_bool	ret;

	ret = True;
	if (!params)
		return (True);
	i = 0;
	while (params[i])
	{
		printf("uwu\n");
		if (!valid_identifier(params[i]))
		{
			ret = False;
			if (ft_printf("unset: `%s`: not a valid identifier\n",
					params[i]) == -1)
				free_and_exit(m, ERR_WRT);
		}
		else
			delete_var(&m->env, params[i]);
		i++;
	}
	return (ret);
}
