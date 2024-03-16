/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 13:07:12 by pipolint          #+#    #+#             */
/*   Updated: 2024/03/15 16:52:57 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <dirent.h>
# include <sys/wait.h>
# include "general.h"
# include "libft.h"
# include "parser.h"

typedef struct s_blt
{
	char	**path;
	char	**env;
	char	*pwd;
	char	*old_pwd;
}	t_blt;

t_bool	echo(char *str, t_bool n);
void	cd(t_blt *b, char *dir);
void	exiting(int code);
void	export(t_env **env, char *variable);

#endif
