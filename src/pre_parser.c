/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehammoud <ehammoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:43:35 by ehammoud          #+#    #+#             */
/*   Updated: 2024/03/01 20:51:33 by ehammoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// We need to handle -- being allowed as a parameter
// Read up on $-
// $_ : The underscore variable is set at shell startup and contains the absolute file name of the shell or script being executed as passed in the argument list. Subsequently, it expands to the last argument to the previous command, after expansion.
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
t_bool	found_in(char c, char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (c == s[i++])
			return (True);
	}
	return (False);
}


t_token	token_type(char *s)
{
	int	i;

	if (!s || found_in(s[0], DIGIT))
		return (Word);
	if (!ft_strncmp(s, "<", -1) || !ft_strncmp(s, "<<", -1)
		|| !ft_strncmp(s, ">", -1) ||!ft_strncmp(s, ">>", -1))
		return (Op_redir);
	if (!ft_strncmp(s, "&&", -1) || !ft_strncmp(s, "||", -1))
		return (Op_logic);
	if (!ft_strncmp(s, "|", -1))
		return (Op_pipe);
	if (!ft_strncmp(s, "(", -1))
		return (Bracket_open);
	if (!ft_strncmp(s, ")", -1))
		return (Bracket_closed);
	i = 0;
	while (s[i])
	{
		if (s[i] != '_' && !found_in(s[i], DIGIT)
			&& !found_in(s[i], LOWERCASE) && !found_in(s[i], UPPERCASE))
			return (Word);
		if (s[++i] == '=')
			return (Assign);
	}
	return (Name);
}

// int	op_occur(char c, char *s)
// {
// 	int	cc;

// 	cc = 0;
// 	if (!s || !*s)
// 		return (-1);
// 	while (*s == c)
// 	{
// 		s++;
// 		cc++;
// 		if (cc > 2)
// 			return (-1);
// 	}
// 	return (cc);
// }

// char	*grab_word(char **s, t_bool start_under_score)
// {
// 	int		i;
// 	int		wlen;
// 	char	*word;

// 	if (!s || !*s || !**s)
// 		return (NULL);
// 	wlen = 0;
// 	while (**s && is_literal(**s, start_under_score || (wlen > 0)))
// 		wlen++;
// 	word = malloc(sizeof(char) * (wlen + 1));
// 	if (!word)
// 		return (NULL);
// 	i = 0;
// 	while (i < wlen)
// 	{
// 		word[i] = *s[i];
// 		i++;
// 	}
// 	word[i] = '\0';
// 	*s += wlen;
// 	return (word);
// }
