/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 22:53:31 by marvin            #+#    #+#             */
/*   Updated: 2024/06/26 13:43:18 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "general.h"
# include <stdlib.h>
# include <sys/wait.h>
# include "libft.h"
# include "parser.h"
# include "builtins.h"
# include "cmd_list.h"
# include "queues.h"
# include "../tmp/tmp_utils.h"
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include "get_next_line.h"
# include "signals.h"

# define NON 0
# define OR_OP 1
# define AND_OP 2
# define PIPE_OP 3
# define SEMICOLON 4
# define DO_NOT_EXECUTE 0
# define WAIT_THEN_EXEC 1
# define IMMEDIATE_EXEC 2
# define CHILD_PROCESS 0
# define SUCCESS 0

typedef struct s_exec
{
	int		ret;
	int		curr_depth;
	int		status_depth;
	int		overall_status;
	int		last_status;
	int		last_op;
	int		*exit_status;
	t_env	**env;
	pid_t	last_pid;
	t_cmd	**cmd_head;
}	t_exec;

typedef struct s_heredoc
{
	t_env 	**env;
	t_cmd 	*cmd;
	t_exec 	*exec; 
	int 	*fds;
	int		i;
}	t_heredoc;

typedef struct s_expand
{
	char	*res;
	char	*var;
	char 	*line; 
	t_env 	**env;
	char	*name;
	char 	**words;
	int		newline;
	char	*var_name;
	char	*delimiter;
}	t_expand;

t_bool	clean_whitespace(t_queue *q);
int		resolve_builtin(t_env **env, t_cmd *cmd, t_exec *exec, t_bool child);
int		execute_commands(t_env **env, t_cmd *cmd, int *status);
t_bool	clean_whitespace(t_queue *q);
int		exec_type(t_exec *exec, t_cmd **cmd);
char	*search_path(t_env **env, t_cmd *cmd);
t_bool	should_exec(t_exec *exec, t_cmd *cmd);
int		wait_for_children(t_exec *exec, int *stand_in, int *stand_out);

t_bool	handle_cmds(t_env **env, t_cmd **cmd, t_exec *exec);
t_bool	execute(t_env **env, t_cmd *cmd, t_exec *exec);
int		execute_cmds(t_env **env, t_cmd **cmds, int *status);

int		dup_and_check(int fd1, int fd2, t_exec *exec);
int		pipe_and_check(int *fds, t_exec *exec);
int		close_and_check(int fd, t_exec *exec);
int		open_and_check(int *fd, char *filename, int mode, t_exec *exec);

int		open_infiles(t_env **env, t_cmd *cmd, t_exec *exec, int *fds);
int		open_outfiles(t_cmd *cmd, t_exec *exec);
void	child_process(t_env **env, t_cmd *cmd, t_exec *exec, int *fds);
int		parent_process(t_cmd *cmd, t_exec *exec, int *fds);
//t_bool	heredoc(t_cmd *cmd, t_exec *exec, int *fds, int i, t_env **env);
t_bool	heredoc(t_heredoc *h);
//char	*expand_variable(char *line, t_env **env, char **words, int *i, char *delim);
char	*expand_variable(t_expand *exp);
t_bool	heredoc_parent(t_cmd **cmd, int *fds, t_exec *exec, t_env **env);
t_bool	heredoc_loop(t_cmd *cmd, t_exec *exec, t_env **env);
//void	heredoc_child(t_cmd *cmd, t_exec *exec, int *fds, int i, t_env **env);
void	heredoc_child(t_heredoc *h);
int		open_outs_and_in(t_cmd *cmd, t_exec *exec);

int		init_expand(t_expand *exp, char *line, t_env **env, char *delimiter);
int		exiting(t_cmd *cmd, char **params, t_exec *exec);

#endif
