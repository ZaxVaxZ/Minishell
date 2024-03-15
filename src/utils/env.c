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

t_env	*new_env_node(char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = key;
	node->value = value;
	//node->key = ft_strdup(key);
	//if (!node->key)
	//{
	//	free(node);
	//	return (NULL);
	//}
	//node->value = ft_strdup(value);
	//if (!node->value)
	//{
	//	if (node->key)
	//		free(key);
	//	free(node);
	//	return (NULL);
	//}
	return (node);
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
		env->key = ft_substr(envp[i], 0, ft_strlen());
		while (envp[i][j] != '\0')
		env->value = ft_substr(envp[i], k, j);
		add_to_env(e, env);
	}
}

void	print_list(t_env *env)
{
	while (env)
	{
		printf("%s ", env->key);
		printf("%s \n", env->value);
		env = env->next;
	}
}
