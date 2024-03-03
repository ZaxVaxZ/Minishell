/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 13:06:04 by pipolint          #+#    #+#             */
/*   Updated: 2024/03/02 13:30:50 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"

void	echo(char *str, t_bool n)
{
	int	stat;

	stat = 0;
	if (n)
	{
		stat = write(1, str, ft_strlen(str));
		if (!stat)
			exit(0);
		exit(1);
	}
	else
	{
		stat = write(1, str, ft_strlen(str));
		stat += write(1, "\n", 1);
		if (!stat)
			exit(stat);
		exit(1);
	}
}
