/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehammoud <ehammoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 13:21:55 by ehammoud          #+#    #+#             */
/*   Updated: 2024/03/01 20:57:28 by ehammoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/*
blank:  A space or tab.
word:   A sequence of characters considered as a single unit by the shell.  Also known as a token.
name:   A word consisting only of alphanumeric characters and underscores, and beginning with an alphabetic character or an underscore.  Also referred to as an identifier.
metacharacter:
		A character that, when unquoted, separates words.  One of the following:
		|  & ; ( ) < > space tab
control operator:
		A token that performs a control function.  It is one of the following symbols:
		|| & && ; ;; ( ) | <newline>
*/


/*
bash.exe"-3.1$ ls > del testing del testing testing del
bash.exe"-3.1$ ls > del testing del testing testing del
bash.exe"-3.1$ echo hi > del testing del testing testing del
bash.exe"-3.1$ echo hi > del testing del testing testinga adsfasdf del
bash.exe"-3.1$ echo > del hi this is a test                           
bash.exe"-3.1$ ls > del -l
bash.exe"-3.1$ > del ls -l
bash.exe"-3.1$ > testing cat < del
bash.exe"-3.1$ > del < testing > del > testing
bash.exe"-3.1$ > del < testing > del >        
bash.exe": syntax error near unexpected token `newline'
bash.exe"-3.1$ > del < testing > del > testing ls
*/

int	add_str_to_queue(t_queue **q, char *str)
{
	t_bool	illegal;
	t_queue	*tmp;

	if (!str)
		return (0);
	tmp = queue_end(q);
	illegal = (is_control_operator(str) && str[0] != LP && (tmp->type == Op_logic
		|| tmp->type == Op_pipe || tmp->type == Bracket_open || tmp->type == Op_redir));
	if ((str[0] == INF || str[0] == OUF) && tmp->type == Op_redir)
		return (0);
	tmp = new_node(str);
	if (!tmp)
		free_queue(q);
	else
	{
		tmp->type = token_type(tmp->s);
		if (illegal)
			tmp->type = Illegal;
		queue(q, tmp);
	}
	free(str);
	if (!(*q))
		return (-1);
	return (ft_strlen(str));
}

t_bool	parse_op(t_queue **q, char **s, char op, int max_occurs)
{
	int	occurs;

	occurs = op_occur(op, *s);
	if (occurs > max_occurs)
		occurs = max_occurs;
	if (occurs == 1 && op == AND)
		return ;
	occurs = add_str_to_queue(q, ft_substr(*s, 0, occurs));
	if (occurs == -1)
		return (False);
	*s += occurs;
	return (True);
}

t_bool	parse_command(t_queue **q, char **s)
{
	char	*prev_s;
	char	*after_parse_word;

	if (q && *q && queue_end(*q)->type == Illegal)
		return (True);
	while (*s == SPACE || *s == TAB)
			(*s)++;
	parse_assigns(q, s);
	prev_s = NULL;
	while (prev_s != *s)
	{
		while (*s == SPACE || *s == TAB)
			(*s)++;
		prev_s = *s;
		parse_op(q, s, INF, 2);
		parse_op(q, s, OUF, 2);
		while (after_parse_word != *s)
			parse_word(q, s);
	}
}

t_bool	parse_control(t_queue **q, char **s)
{
	char	*prev_s;

	if (q && *q && queue_end(*q)->type == Illegal)
		return (True);
	prev_s = NULL;
	while (prev_s != *s)
	{
		while (*s == SPACE || *s == TAB)
			(*s)++;
		prev_s = *s;
		parse_op(q, s, DS, 1);
		parse_op(q, s, LP, 1);
		parse_op(q, s, AND, 2);
		parse_op(q, s, PIPE, 2);
		parse_op(q, s, RP, 1);
		parse_op(q, s, SC, 1);
		parse_op(q, s, NL, 1);
	}
}

t_queue	*parse(char *s)
{
	char	*prev_s;
	t_queue	*q;

	q = NULL;
	prev_s = NULL;
	while (prev_s != s)
	{
		prev_s = s;
		parse_command(&q, &s);
		parse_control(&q, &s);
	}
}


// int	add_str_to_queue(t_queue **q, char *str)
// {
// 	t_queue	*tmp;

// 	if (!str)
// 		return (0);
// 	tmp = queue_end(q);
// 	if (is_control_operator(str) && str[0] != LP && (tmp->type == Op_logic
// 		|| tmp->type == Op_pipe || tmp->type == Bracket_open || tmp->type == Op_redir))
// 		return (0);
// 	if ((str[0] == INF || str[0] == OUF) && tmp->type == Op_redir)
// 		return (0);
// 	tmp = new_node(str);
// 	if (!tmp)
// 		free_queue(q);
// 	else
// 	{
// 		tmp->type = token_type(tmp->s);
// 		queue(q, tmp);
// 	}
// 	free(str);
// 	if (!(*q))
// 		return (-1);
// 	return (ft_strlen(str));
// }

// t_bool	extract_op(t_queue **q, char **s, char op, int max_occurs)
// {
// 	int		occurs;

// 	occurs = op_occur(op, s);
// 	if (occurs > max_occurs)
// 		occurs = max_occurs;
// 	if (occurs == 1 && op == '&')
// 		return (True);
// 	occurs = add_str_to_queue(q, ft_substr(*s, 0, occurs));
// 	if (occurs == -1)
// 		return (False);
// 	*s += occurs;
// 	return (True);
// }

// t_bool	parse_metacharacters(t_queue **q, char **str)
// {
// 	if (!extract_op(q, str, LP, 1))
// 		return (False);
// 	if (!extract_op(q, str, RP, 1))
// 		return (False);
// 	if (!extract_op(q, str, DS, 1))
// 		return (False);
// 	if (!extract_op(q, str, PIPE, 2))
// 		return (False);
// 	if (!extract_op(q, str, AND, 2))
// 		return (False);
// 	if (!extract_op(q, str, INF, 2))
// 		return (False);
// 	if (!extract_op(q, str, OUF, 2))
// 		return (False);
// 	return (True);
// }

// t_queue	*parse(char *s)
// {
// 	char	*prev_s;
// 	t_queue	*q;

// 	q = NULL;
// 	while (s)
// 	{
// 		while (*s == SPACE || *s == TAB)
// 			s++;
// 		prev_s = s;
// 		parse_metacharacters(&q, &s);
// 		if (s == prev_s)
// 			grab_word(&q, &s);
// 		// fix here
// 	}
// }