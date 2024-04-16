#include "executor.h"

t_bool	heredoc_child(t_env **env, t_cmd *cmd, t_exec *exec, int *fds)
{
	char	*line;

	while (1)
	{
		line = get_next_line(STDIN_FILENO);
		if (!ft_strncmp(line, cmd->input, ft_strlen(line) - 1)
			&&ft_strlen(line) - 1 == ft_strlen(cmd->input))
			break ;
		if (write(fds[1], line, ft_strlen(line)) == -1)
			return (False);
		free(line);
	}
	close(fds[1]);
	if (line)
		free(line);
	return (True);
}

t_bool	heredoc_parent(int *fds)
{
	if (dup_and_check(fds[0], STDIN_FILENO) == -1)
		return (False);
	close(fds[0]);
	return (True);
}

t_bool	heredoc(t_env **env, t_cmd *cmd, t_exec *exec, int *fds)
{
	if (heredoc_child(env, cmd, exec, fds) == False)
		return (False);
	if (heredoc_parent(fds) == False)
		return (False);
	return (True);
}