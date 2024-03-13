/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 13:31:22 by pipolint          #+#    #+#             */
/*   Updated: 2024/03/13 15:28:46 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/builtins.h"

void	cd(t_builtins *b, char *dir)
{
	b->old_pwd = b->pwd;
	//b->pwd = getenv("PWD");
	if (chdir(dir) == -1)
		perror(NULL);
	printf("Old PWD: %s\n", b->old_pwd);
	printf("Current PWD: %s\n", b->pwd);
}
