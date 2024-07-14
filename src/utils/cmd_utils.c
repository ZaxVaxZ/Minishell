/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 05:23:45 by marvin            #+#    #+#             */
/*   Updated: 2024/07/14 13:44:48 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd_list.h"
#include "ft_printf.h"
#include <stdio.h>

/* -----------------------
 * Functions in the file:
 *   count_words()
 *   free_and_return()
 *   is_separator()
 * 	 count_out_redirs()
 *   prep_cmd()
 * -----------------------*/

/// @brief Count all parameters that are part of one command
/// @param q The parser queue
/// @return Returns their count
static int	count_words(t_queue *q)
{
	int		count;

	count = 0;
	while (q && !is_separator(q))
	{
		if (q->type == Word)
			count++;
		q = q->next;
	}
	return (count);
}
/// @brief counts the infile and outfile redirs and sets their counters accordingly
/// @param q the queue to traverse
/// @param in the pointer to the in count
/// @param out the pointer to the out count
/// @return returns 0 if no infile or outfile; 1 for infile; 2 for out; 3 for both
int	count_redirs(t_queue *q, int *in, int *out)
{
	int	has_inf;
	int	has_outf;

	has_inf = 0;
	has_outf = 0;
	while (q && !is_separator(q))
	{
		if (q->type == Op_redir && q->s[0] == '>' && q->next)
		{
			(*out)++;
			has_outf = 2;
		}
		else if (q->type == Op_redir && q->s[0] == '<' && q->next)
		{
			(*in)++;
			has_inf = 1;
		}
		q = q->next;
	}
	return (has_inf + has_outf);
}

t_bool	free_and_return(t_queue **q, t_env **env, t_cmd **cmds, t_cmd *cmd)
{
	free_queue(q);
	free_cmd(cmds);
	free_env(env);
	free_cmd_node(cmd);
	return (False);
}

t_bool	prep_cmd(t_queue *q, t_cmd **node)
{
	char	**params;
	int		in;
	int		out;

	params = malloc(sizeof(char *) * (count_words(q) + 1));
	if (!params)
		return (False);
	*node = new_cmd_node(params, &q);
	in = 0;
	out = 0;
	if (!*node)
	{
		free(params);
		return (False);
	}
	params[0] = NULL;
	if (!count_redirs(q, &in, &out))
		return (True);
	if (in)
	{
		(*node)->in_flags = malloc(sizeof(int) * in);
		if (!(*node)->in_flags)
			return (free_cmd_node(*node));
		(*node)->infiles = malloc(sizeof(char *) * (in + 1));
		if (!(*node)->infiles)
			return (free_cmd_node(*node));
	}
	if (out)
	{
		(*node)->out_flags = malloc(sizeof(int) * out);
		if (!(*node)->out_flags)
			return (free_cmd_node(*node));
		(*node)->outfiles = malloc(sizeof(char *) * (out + 1));
		if (!(*node)->outfiles)
			return (free_cmd_node(*node));
	}
	return (True);
}

t_bool	is_separator(t_queue *node)
{
	return (node->type == Op_pipe || node->type == Op_logic
		|| node->type == Semicolon
		|| node->type == Bracket_open || node->type == Bracket_closed);
}
