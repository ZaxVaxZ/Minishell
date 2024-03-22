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

	i = (n == True);
	while (s[i])
	{
		if (printf("%s", s[i]) == -1 || (s[i + 1] && printf(" ") == -1))
			return (False);
		i++;
	}
	if (!n && printf("\n") == -1)
		return (False);
	return (True);
}
