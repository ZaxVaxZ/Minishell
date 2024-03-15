/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 22:53:20 by marvin            #+#    #+#             */
/*   Updated: 2024/03/14 22:53:20 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	count_words(t_queue *q)
{
	int		count;
	t_queue	*prev;

	count = 0;
	prev = NULL;
	while (q && q->type != Op_logic && q->type != Semicolon
		&& q->type != Op_pipe)
	{
		if (q->type == Word && (!prev
				|| (prev->type != Op_redir && prev->type != Assign)))
			count++;
		prev = q;
		q = q->next;
	}
	return (count);
}

int	execute(t_queue *q, t_command *cmd)
{
	int		i;
	int		cmds;
	char	*tmp;

	(void)q;
	cmds = count_words(q);
	cmd->params = malloc(sizeof(char *) * (cmds + 1));
	i = 0;
	cmd->input = NULL;
	cmd->output = NULL;
	while (q && q->type != Op_logic && q->type != Semicolon
		&& q->type != Op_pipe)
	{
		if (q->type == Assign && q->next && q->next->type == Word)
		{
			tmp = ft_substr(q->s, 0, ft_strlen(q->s) - 1);
			if (!tmp)
				return (free_and_return());
			setenv(tmp, q->next->s);
			q = q->next;
		}
		else if (q->type == Op_redir)
			q = q->next;
		else if (q->type == Word)
		{
			cmd->params[i] = ft_strdup(q->s);
			if (!cmd->params[i++])
				return (free_and_return());
		}
		q = q->next;
	}
	cmd->params[i] = NULL;
	
	return (0);
}
