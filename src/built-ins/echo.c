/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 13:06:04 by pipolint          #+#    #+#             */
/*   Updated: 2024/03/18 11:50:54 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

t_bool	echo(char **s, t_bool n)
{
	int	i;
	char	*str;

	str = (*s);
	if (!str)
		return (True);
	i = 0;
	while (str[i])
	{
		//if (printf("%s", str[i]) == -1)
		if (write(1, &str[i], 1) == -1)
			return (False);
		//if (str[i + 1] && printf(" ") == -1)
		//if (str[i + 1] && write(1, " ", 1) == -1)
		//	return (False);
		i++;
	}
	//if (!n && printf("\n") == -1)
	if (!n && write(1, "\n", 1) == -1)
		return (False);
	return (True);
}
