/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 13:06:04 by pipolint          #+#    #+#             */
/*   Updated: 2024/07/24 21:20:29 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

t_bool	echo(t_main *m, char **s)
{
	int		i;
	int		j;
	t_bool	n;

	n = (s[0] && s[0][0] == '-');
	j = 1;
	while (n && s[0][j])
	{
		if (s[0][j] != 'n')
			n = False;
		j++;
	}
	i = (n == True);
	while (s && s[i])
	{
		if (ft_printf("%s", s[i]) == -1 || (s[i + 1] && ft_printf(" ") == -1))
			free_and_exit(m, ERR_WRT);
		i++;
	}
	if (!n && ft_printf("\n") == -1)
		free_and_exit(m, ERR_WRT);
	return (True);
}
