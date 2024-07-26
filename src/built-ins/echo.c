/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehammoud <ehammoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 13:06:04 by pipolint          #+#    #+#             */
/*   Updated: 2024/07/26 16:39:51 by ehammoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

t_bool	is_n_flag(char *s)
{
	int		i;
	t_bool	ret;

	ret = (s[0] == '-');
	i = 1;
	while (ret && s[i])
	{
		if (s[i++] != 'n')
			ret = False;
	}
	return (ret);
}

t_bool	echo(t_main *m, char **s)
{
	int		i;
	int		j;

	i = 0;
	while (s[i] && is_n_flag(s[i]))
		i++;
	j = i;
	while (s && s[i])
	{
		if (ft_printf("%s", s[i]) == -1 || (s[i + 1] && ft_printf(" ") == -1))
			free_and_exit(m, ERR_WRT);
		i++;
	}
	if (!j && ft_printf("\n") == -1)
		free_and_exit(m, ERR_WRT);
	return (True);
}
