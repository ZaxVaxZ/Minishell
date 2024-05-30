/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 12:18:21 by pipolint          #+#    #+#             */
/*   Updated: 2024/05/30 16:05:25 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int	init_expand(t_expand *exp, char *line, t_env **env, char *delimiter)
{
	exp->line = line;
	exp->env = env;
	exp->delimiter = delimiter;
	exp->words = ft_split(line, ' ');
	if (!exp->words)
		return (-1);
	exp->var = NULL;
	exp->res = NULL;
	exp->var_name = NULL;
	exp->newline = 0;
	return (1);
}

void	set_name_var(t_expand *exp, int i, int *j)
{
	if (exp->words[i][*j] == '$' && ++(*j))
	{
		if (exp->words[i][*j] == '$')
			(*j)++;
		else if (!is_valid_var_char(exp->words[i][*j]))
		{
			exp->name = ft_strdup("$");
			exp->var = ft_strjoin(exp->name, &exp->words[i][*j]);
		}
		else
		{
			exp->name = get_var_name(exp->words[i], j, &exp->newline);
			exp->var = get_var((*exp->env), exp->name);
			free(exp->name);
		}
	}
	else if (exp->words[i][*j] && exp->words[i][*j] != '\n')
		exp->var = get_word(exp->words[i], j, exp->newline);
	*j -= (exp->words[i][*j] == '$');
	if (exp->var)
		exp->res = ft_strjoin(exp->res, exp->var);
	if (exp->newline)
		exp->res = ft_strjoin(exp->res, "\n");
}

char	*expand_variable(t_expand *exp)
{
	int		i;
	int		j;

	if (!exp->line)
		return (NULL);
	if (!ft_strncmp(exp->line, exp->delimiter, ft_strlen(exp->line) - 1)
		&& ft_strlen(exp->line) - 1 == ft_strlen(exp->delimiter))
		return (ft_strjoin(exp->delimiter, "\n"));
	i = -1;
	while (exp->words[++i])
	{
		j = -1;
		while (exp->words[i][++j])
		{
			exp->newline = (exp->words[i][j] == '\n');
			set_name_var(exp, i, &j);
			j -= (exp->words[i][j] == '$');
			if (exp->words[i + 1])
				exp->res = ft_strjoin(exp->res, " ");
			if (exp->words[i][j] == '\0')
				break;
		}
	}
	ft_freeup(exp->words);
	return (exp->res);
}
