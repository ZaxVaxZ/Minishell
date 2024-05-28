/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 12:18:21 by pipolint          #+#    #+#             */
/*   Updated: 2024/05/28 16:50:42 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "queues.h"
#include "executor.h"

char	*get_var_name(char *word, int *j, int *newline)
{
	char	*name;
	int		k;

	k = *j;
	while (word[*j] && is_valid_var_char(word[*j]))
	{
		(*j)++;
		if (word[*j] == '\n')
			*newline = 1;
	}
	name = malloc(sizeof(char) * ((*j - k) + 1));
	if (!name)
		return (NULL);
	*j = k;
	k = 0;
	while (word[*j] && is_valid_var_char(word[*j]))
		name[k++] = word[(*j)++];
	name[k] = '\0';
	return (name);
}

char	*get_word(char *word, int *j, int newline)
{
	char	*w;
	int		i;

	i = *j;
	// while (word[*j] != '\0' && word[*j] != '$')
	while (word[*j] != '\0')
	{
		if (word[*j] == '$' || word[*j] == '\n')
			break ;
		(*j)++;
	}
	w = malloc(sizeof(char) * ((*j - i) + (word[*j] == '\n') + 1));
	if (!w)
		return (NULL);
	*j = i;
	i = 0;
	while (word[*j] && word[*j] != '$')
	{
		if (newline && word[(*j)] == '\n')
			break ;
		else
			w[i++] = word[(*j)++];
	}
	w[i] = '\0';
	return (w);
}

char	*expand_variable(char *line, t_env **env, char **words, int *i)
{
	char	*res;
	char	*name;
	int		j;
	char	*var;
	int		newline;
	char	*del;

	if (!line)
		return (NULL);
	(*i) = -1;
	newline = 0;
	words = ft_split(line, ' ');
	if (words[0] && words[0][0] == '$' && !words[1])
	{
		del = ft_strdup(words[0]);
		ft_freeup(words);
		return (del);
	}
	if (!words)
		return (NULL);
	var = NULL;
	name = NULL;
	res = NULL;
	while (words[++(*i)])
	{
		j = -1;
		while (words[(*i)][++j])
		{
			newline = (words[(*i)][j] == '\n');
			if (words[(*i)][j] == '$' && ++j)
			{
				if (words[*i][j] == '$')
					j++;
				else if (!is_valid_var_char(words[(*i)][j]))
				{
					name = ft_strdup("$");
					var = ft_strjoin(name, &words[(*i)][j]);
				}
				else
				{
					name = get_var_name(words[(*i)], &j, &newline);
					var = get_var(*env, name);
					free(name);
				}
			}
			else if (words[(*i)][j] && words[(*i)][j] != '\n')
				var = get_word(words[(*i)], &j, newline);
			j -= (words[(*i)][j] == '$');
			if (var)
				res = ft_strjoin(res, var);
			if (newline)
				res = ft_strjoin(res, "\n");
			if (words[(*i) + 1])
				res = ft_strjoin(res, " ");
			if (words[(*i)][j] == '\0')
				break;
		}
	}
	ft_freeup(words);
	return (res);
}
