#include "parser.h"

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

int	op_occur(char c, char *s)
{
	int	cc;

	cc = 0;
	if (!s || !*s)
		return (-1);
	while (*s == c)
	{
		s++;
		cc++;
	}
	return (cc);
}

t_bool	is_control_operator(char *str)
{
	return (str[0] == PIPE || str[0] == AND || str[0] == SC || str[0] == LP || str[0] == RP);
}