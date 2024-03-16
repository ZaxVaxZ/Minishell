#include "builtins.h"
#include "env.h"

void	export(t_env **env, char *variable)
{
	t_env	*trav;
	int		flag;

	trav = (*env);
	flag = 1;
	while (trav)
	{
		if (!ft_strncmp(trav->key, variable, ft_strlen(variable) - ft_strlen(ft_strchr(variable, '='))))
		{
			flag = 0;
			trav->value = variable;
			break ;
		}
		trav = trav->next;
	}
	if (flag)
	{
		trav = malloc(sizeof(t_env));
		if (!trav)
			return ;
		trav->key = ft_substr(variable, 0, ft_strlen(ft_strchr(variable, '=')));
		trav->value = ft_substr(variable, ft_strlen(ft_strchr(variable, '=')), ft_strlen(variable));
		add_to_env(env, trav);
	}
}
