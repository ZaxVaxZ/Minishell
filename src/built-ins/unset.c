/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehammoud <ehammoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 03:56:02 by marvin            #+#    #+#             */
/*   Updated: 2024/07/26 19:04:59 by ehammoud         ###   ########.fr       */
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
		if (!valid_identifier(params[i]))
		{
			ret = False;
			errlog(m, "unset: `", params[i], "`: not a valid identifier\n");
		}
		else
			delete_var(&m->env, params[i]);
		i++;
	}
	return (ret);
}
