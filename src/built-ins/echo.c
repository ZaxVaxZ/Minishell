/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 13:06:04 by pipolint          #+#    #+#             */
/*   Updated: 2024/03/15 11:40:31 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

t_bool	echo(char **str, t_bool n)
{
	int	i;

	if (!str)
		return (True);
	i = 0;
	while (str[i])
	{
		if (printf("%s", str[i]) == -1)
			return (False);
		if (str[i + 1] && printf(" ") == -1)
			return (False);
		i++;
	}
	if (!n && printf("\n") == -1)
		return (False);
	return (True);
}
