/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 15:14:13 by pipolint          #+#    #+#             */
/*   Updated: 2024/03/15 17:03:43 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include <stdlib.h>
# include "general.h"
# include "libft.h"

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

t_env	*new_env_node(char *key, char *value);
void	add_env_node(t_env **env, t_env *to_add);
void	free_env_node(t_env *node);
int		env_size(t_env *env);
t_bool	free_env(t_env **env);
void	delete_var(t_env **env, char *key);
t_env	*to_env_list(char **strs);
char	**to_char_arr(t_env **env);

#endif
