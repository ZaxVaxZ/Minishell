/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 22:53:31 by marvin            #+#    #+#             */
/*   Updated: 2024/07/25 16:22:44 by pipolint         ###   ########.fr       */
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
//# include "../tmp/tmp_utils.h"
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include "get_next_line.h"
# include "signals.h"
# include "general.h"

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
//# define READEND 0
//# define WRITEEND 1

t_bool	clean_whitespace(t_main *m);
int		resolve_builtin(t_main *m, t_cmd *cmd, t_exec *exec, t_bool child);
int		execute_commands(t_main *m);
int		exec_type(t_exec *exec, t_cmd **cmd);
char	*search_path(t_env **env, t_cmd *cmd);
t_bool	should_exec(t_exec *exec, t_cmd *cmd);
int		wait_for_children(t_exec *exec);

t_bool	handle_cmds(t_main *m, t_cmd **cmd, t_exec *exec);
t_bool	execute(t_main *m, t_cmd *cmd, t_exec *exec);

int		dup_and_check(int fd1, int fd2, t_exec *exec);
int		pipe_and_check(int *fds, t_exec *exec);
int		close_and_check(int fd, t_exec *exec);
int		open_and_check(int *fd, char *filename, int mode, t_exec *exec);

int		open_infiles(t_env **env, t_cmd *cmd, t_exec *exec, int *fds);
int		open_outfiles(t_cmd *cmd, t_exec *exec);
void	child_process(t_main *m, t_cmd *cmd, t_exec *exec, int *fds);
int		parent_process(t_cmd *cmd, t_exec *exec, int *fds);
t_bool	heredoc(t_heredoc *h);
t_bool	heredoc_parent(t_main *m, t_cmd **cmd, int *fds, t_exec *exec);
t_bool	heredoc_loop(t_main *m, t_cmd *cmd, t_exec *exec, t_env **env);
void	heredoc_child(t_heredoc *h);
t_bool	handle_heredoc_sigint(t_main *m, t_cmd **cmd, int *fds, t_exec *exec);
void	write_exp_str(t_heredoc *h, char *line);
int		open_outs_and_in(t_main *m, t_cmd *cmd, t_exec *exec);

int		exiting(t_main *m, t_cmd *cmd, char **params, t_exec *exec);

t_bool	exec_cmd(t_main *m, t_cmd **cmd, t_exec *exec, int *fds);
t_bool	close_heredoc_failed(t_main *m, t_cmd **cmd, t_exec *exec, int *fds);
int		pipe_and_set_fds(t_exec *exec, int *fds, t_cmd **cmd);
t_bool	deal_with_ret(t_main *m, t_cmd **cmd, t_exec *exec, int *fds);
t_bool	open_heredocs(t_main *m, t_exec *exec, t_cmd *heredoc);
int		post_command(t_main *m, t_exec *exec);
int		close_pipes(t_main *m, t_exec *exec, t_cmd *cmd, int *fds);
int		should_break_heredoc(t_heredoc *h, char *line);

#endif
