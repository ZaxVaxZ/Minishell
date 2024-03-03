/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehammoud <ehammoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:43:35 by ehammoud          #+#    #+#             */
/*   Updated: 2024/03/02 20:38:24 by ehammoud         ###   ########.fr       */
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
		if (s[i] != UNDERSCORE && !found_in(s[i], DIGIT)
			&& !found_in(s[i], LOWERCASE) && !found_in(s[i], UPPERCASE))
			return (Word);
		if (s[++i] == '=')
			return (Assign);
	}
	return (Name);
}

// Has to be a combination of: UPPERCASE, LOWERCASE, UNDERSCORE and DIGIT (but no DIGIT in position 0).... until reaching EQUAL... then parse everything till the next SPACE or TAB
// If the strict rule is broken then just return (True);
// If EQUAL not found, return (True);
// If all goes perfectly, add the substr until the whitespace into the queue
t_bool	grab_assign(t_queue **q, char **s)
{
	int		wlen;
	t_bool	found_eq;

	if (!s || !*s || !**s || found_in(**s, DIGIT))
		return (True);
	found_eq = False;
	wlen = 0;
	while (*s[wlen] && *s[wlen] != SPACE && *s[wlen] != TAB
		&& !is_meta_char(*s + wlen))
	{
		if (!found_eq && !found_in(*s[wlen], UPPERCASE)
			&& !found_in(*s[wlen], LOWERCASE) 
			&& !found_in(*s[wlen], DIGIT) && *s[wlen] != UNDERSCORE)
			return (True);
		wlen++;
		found_eq = (!found_eq && *s[wlen] == EQUAL);
	}
	if (!found_eq)
		return (True);
	if (!add_str_to_queue(q, ft_substr(*s, 0, wlen)))
		return (False);
	*s += wlen;
	return (True);
}

t_bool	parse_assigns(t_queue **q, char **s)
{
	char	*prev_s;

	if (!s || !*s || !**s || !q)
		return (True);
	prev_s = NULL;
	while (prev_s != *s)
	{
		while (**s == SPACE || **s == TAB)
			(*s)++;
		prev_s = *s;
		if (!grab_assign(q, s))
			return (False);
	}
	return (True);
}

t_bool	grab_word(t_queue **q, char **s)
{
	int		wlen;
	t_bool	valid_name;
	
	wlen = 0;
	valid_name = True;
	if (found_in(*s[wlen], DIGIT))
		valid_name = False;
	while (*s[wlen] && *s[wlen] != SPACE && *s[wlen] != TAB
		&& !is_meta_char(*s + wlen))
	{
		if (!found_in(*s[wlen], DIGIT) && !found_in(*s[wlen], LOWERCASE)
			&& *s[wlen] != UNDERSCORE && !found_in(*s[wlen], UPPERCASE)
			&& valid_name)
			valid_name = False;
		wlen++;
	}
	if (!add_str_to_queue(q, ft_substr(*s, 0, wlen)))
		return (False);
	*s += wlen;
	return (True);
}

//t_token	grab_word(t_queue **q, char **s)
//{
//	int		i;
//	int		wlen;
//	char	*word;
//	t_token	ret;

//	if (!s || !*s || !**s || is_control_op(**s) || **s == OUF || **s == INF)
//		return (Illegal);
//	ret = Name;
//	if (!found_in(**s, UPPERCASE) && !found_in(**s, LOWERCASE) && **s != UNDERSCORE)
//		ret = Word;
//	wlen = 0;
//	while (*s[wlen] && *s[wlen] != ' ' && *s[wlen] != '\t')
//	{
//		if (*s[wlen] == EQUAL && wlen > 0 && ret != Word)
//			ret = Assign;
//		//Scrutinize Name vs Assign vs Word
//		wlen++;
//	}
//	//word = malloc(sizeof(char) * (wlen + 1));
//	//if (!word)
//	//	return (NULL);
//	//i = 0;
//	//while (i < wlen)
//	//{
//	//	word[i] = *s[i];
//	//	i++;
//	//}
//	//word[i] = '\0';
//	//*s += wlen;
//	//return (word);
//}
