#include "executor.h"
#include "cmd_list.h"

int	dup_and_check(int fd1, int fd2)
{
	if (dup2(fd1, fd2) == -1)
	{
		perror(NULL);
		return (-1);
	}
	return (1);
}

int	pipe_and_check(int *fds)
{
	if (pipe(fds) == -1)
	{
		perror(NULL);
		return (-1);
	}
	return (1);
}

/// @brief this will execute the command and open/close pipes accordingly
/// @param cmd the cmd node
/// @param exec the exec struct
/// @param fds the piped fds
/// @return will exit depending on whether the commaned succeeded or failed
void	child_process(t_env **env, t_cmd *cmd, t_exec *exec, int *fds)
{
	int	ret;

	if (cmd->after == PIPE_OP || cmd->before == PIPE_OP)
	{
		close(fds[0]);
		if (dup_and_check(fds[1], STDOUT_FILENO) == -1)
		{
			exec->last_status = EXIT_FAILURE;
			perror(NULL);
			exit(exec->last_status);
		}
		close(fds[1]);
	}
	ret = resolve_builtin(env, cmd, exec, True);
	if (ret == 0)
	{
		execute(env, cmd);
		exec->last_status = cmd->status;
	}
	else if (ret == -1)
		exec->last_status = EXIT_FAILURE;
	else
		exec->last_status = SUCCESS;
	cmd->status = SUCCESS;
	exit(exec->last_status);
}

void	parent_process(t_cmd *cmd, int *fds)
{
	if (cmd->before == PIPE_OP || cmd->after == PIPE_OP)
	{
		close(fds[1]);
		if (dup_and_check(fds[0], STDIN_FILENO) == -1)
			exit(EXIT_FAILURE);
		close(fds[0]);
	}
}