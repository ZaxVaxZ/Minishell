/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 03:55:54 by marvin            #+#    #+#             */
/*   Updated: 2024/07/24 21:20:16 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

t_bool	pwd(t_main *m)
{
	int		ret;
	char	*tmp;

	tmp = getcwd(NULL, 0);
	ret = ft_printf("%s\n", tmp);
	if (ret == -1)
		free_and_exit(m, ERR_WRT);
	if (tmp)
		free(tmp);
	return (ret != -1);
}
