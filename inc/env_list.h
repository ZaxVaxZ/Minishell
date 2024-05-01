/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 15:14:13 by pipolint          #+#    #+#             */
/*   Updated: 2024/05/01 20:49:21 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_LIST_H
# define ENV_LIST_H

# include <stdlib.h>
# include "general.h"
# include "libft.h"

t_env	*new_env_node(char *key, char *value, t_bool exported);
void	add_env_node(t_env **env, t_env *to_add);
void	free_env_node(t_env *node);
int		env_size(t_env *env);
t_bool	free_env(t_env **env);
t_bool	add_var(t_env **env, char *key, char *value);
t_bool	set_var(t_env **env, char *key, char *value, t_bool exported);
char	*get_var(t_env *env, char *key);
void	delete_var(t_env **env, char *key);
t_bool	export_var(t_env **e, char *str);
t_env	*to_env_list(char **strs);
char	**to_char_arr(t_env **env);
void	print_sorted_env(t_env *env);

#endif
