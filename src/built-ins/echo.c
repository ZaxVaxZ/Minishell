/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 13:06:04 by pipolint          #+#    #+#             */
/*   Updated: 2024/07/19 00:41:38 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

t_bool	echo(char **s)
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
			return (False);
		i++;
	}
	if (!n && ft_printf("\n") == -1)
		return (False);
	return (True);
}
