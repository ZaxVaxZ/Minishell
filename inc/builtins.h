/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 13:07:12 by pipolint          #+#    #+#             */
/*   Updated: 2024/04/03 16:06:10 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/stat.h>
# include "general.h"
# include "libft.h"
# include "parser.h"
# include "env_list.h"
# include "ft_printf.h"

int		cd(char *pwd, char *dir, t_env **env);
void	exiting(int code);
t_bool	unset(t_env **env, char **params);
t_bool	pwd(t_env *env);
t_bool	print_env(t_env *env);
t_bool	echo(char **str, t_bool n);
t_bool	export_cmd(t_env **env, char **variable);
t_bool	valid_identifier(char *id);

#endif
