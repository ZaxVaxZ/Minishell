/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_tools.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 18:25:51 by marvin            #+#    #+#             */
/*   Updated: 2024/03/16 18:25:51 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_list.h"

/* -----------------------
 * Functions in the file:
 *   set_var()
 *   get_var()
 *   delete_var()
 *   to_env_list()
 *   to_char_arr()
 * -----------------------*/


t_bool	set_var(t_env **env, char *key, char *value)
{
	t_env	*tmp;

	if (!env || !key || !value)
		return (True);
	if (!get_var(*env, key))
	{
		tmp = new_env_node(key, value);
		if (!tmp)
			return (free_env(env));
		add_env_node(env, tmp);
		return (True);
	}
	tmp = *env;
	while (tmp)
	{
		if (!ft_strncmp(tmp->key, key, -1))
		{
			free(tmp->value);
			tmp->value = ft_strdup(value);
			if (!tmp->value)
				return (free_env(env));
			return (True);
		}
		tmp = tmp->next;
	}
}

char	*get_var(t_env *env, char *key)
{
	while (env)
	{
		if (!ft_strncmp(env->key, key, -1))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}


void	delete_var(t_env **env, char *key)
{
	t_env	*prev;
	t_env	*curr;

	if (!env || !*env || !key || !*key)
		return ;
	prev = *env;
	if (!ft_strncmp(prev->key, key, -1))
	{
		*env = (*env)->next;
		free_env_node(prev);
		return ;
	}
	curr = prev->next;
	while (curr)
	{
		if (!ft_strncmp(curr->key, key, -1))
		{
			prev->next = curr->next;
			free_env_node(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

t_env	*to_env_list(char **strs)
{
	int		i;
	char	*eqp;
	t_env	*tmp;
	t_env	*env;

	i = 0;
	while (strs[i])
	{
		tmp = malloc(sizeof(t_env));
		if (!tmp)
		{
			free_env(env);
			return ;
		}
		eqp = ft_strchr(strs[i], '=');
		tmp->key = ft_substr(strs[i], 0, eqp - strs[i]);
		tmp->value = ft_substr(strs[i], eqp - strs[i] + 1, ft_strlen(eqp + 1));
		add_to_env(env, tmp);
	}
}

char	**to_char_arr(t_env **env)
{
	char	**envp;
	int		i;
	t_env	*env;

	env = NULL;
	envp = malloc(sizeof(char *) * env_size(env) + 1);
	if (!envp)
		return (NULL);
	i = 0;
	while (env)
	{
		envp[i] = malloc(sizeof(char) * (ft_strlen(env->key) + ft_strlen(env->value)) + 1);
		if (!envp[i])
		{
			free_env(env);
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
