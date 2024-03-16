/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 15:13:59 by pipolint          #+#    #+#             */
/*   Updated: 2024/03/15 17:09:01 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include <stdio.h>

// t_env	*new_env_node(char *key, char *value)
// {
// 	t_env	*node;
//
// 	node = malloc(sizeof(t_env));
// 	if (!node)
// 		return (NULL);
// 	node->key = key;
// 	node->value = value;
// 	return (node);
// }

int	env_size(t_env *env)
{
	int	size;

	size = 0;
	while (env)
	{
		size++;
		env = env->next;
	}
	return (size);
}

t_bool	free_env(t_env **q)
{
	t_env	*tmp;

	while (q && *q)
	{
		tmp = *q;
		*q = (*q)->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
	return (False);
}

void	add_to_env(t_env **env, t_env *to_add)
{
	t_env	*trav;

	if (!to_add)
		return ;
	if (!(*env))
	{
		(*env) = to_add;
		return ;
	}
	trav = (*env);
	while (trav->next)
		trav = trav->next;
	trav->next = to_add;
}

void	env_to_list(char **envp, t_env **e)
{
	t_env	*env;
	int		i;

	i = -1;
	while (envp[++i])
	{
		env = malloc(sizeof(t_env));
		if (!env)
		{
			free_env(e);
			return ;
		}
		env->key = ft_substr(envp[i], 0, ft_strlen(ft_strchr(envp[i], '=')));
		env->value = ft_substr(envp[i], ft_strlen(ft_strchr(envp[i], '=')), ft_strlen(envp[i]));
		add_to_env(e, env);
	}
}

char	**list_to_env(t_env **e)
{
	char	**envp;
	int		i;
	t_env	*env;

	env = (*e);
	envp = malloc(sizeof(char *) * env_size(env) + 1);
	if (!envp)
		return (NULL);
	i = 0;
	while (env)
	{
		envp[i] = malloc(sizeof(char) * (ft_strlen(env->key) + ft_strlen(env->value)) + 1);
		if (!envp[i])
		{
			free_env(e);
			return (NULL);
		}
		envp[i] = ft_strjoin(env->key, env->value);
		if (!envp[i])
		{
			free_env(e);
			return (NULL);
		}
		env = env->next;
		i++;
	}
	envp[i + 1] = 0;
	return (envp);
}

void	print_list(t_env *env)
{
	while (env)
	{
		printf("%s", env->key);
		printf("%s\n", env->value);
		env = env->next;
	}
}
