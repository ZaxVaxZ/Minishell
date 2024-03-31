/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 13:06:04 by pipolint          #+#    #+#             */
/*   Updated: 2024/03/31 13:27:13 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

t_bool	echo(char **s, t_bool n)
{
	int	i;

	i = (n == True);
	while (s && s[i])
	{
		if (printf("%s", s[i]) == -1 || (s[i + 1] && ft_printf(" ") == -1))
			return (False);
		i++;
	}
	if (!n && ft_printf("\n") == -1)
		return (False);
	return (True);
}
