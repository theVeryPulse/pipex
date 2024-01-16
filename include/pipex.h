/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Philip <juli@student.42london.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 15:35:38 by Philip            #+#    #+#             */
/*   Updated: 2024/01/16 22:45:38 by Philip           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft.h"
# include <errno.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdarg.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>

# define NO_PERMIT (1) 

typedef struct s_cmd
{
	char	**argv;
	char	*path;
	char	**envp;
	bool	is_delimiter;
}	t_cmd;

// limiter set as the first cmd in the array with is_limiter as true;
// In exec_first_command function, checks if first command is limiter and use 
// correspoding codes.

int		*build_pipes(int pipe_count);
void	check_cmds(t_cmd *cmds, int cmd_count);
void	close_all_pipes(int	*pipes);
void	exec_commands(int argc, char **argv, char **envp);
void	exec_first_command(t_cmd *cmds, int *pipes, int input_fd);
pid_t	exec_last_command(t_cmd *cmds, int *pipes, char *outfile);
void	exec_middle_commands(t_cmd *cmds, int *pipes, int cmd_count);
int		exit_status_of_last_cmd(pid_t id_last);
t_cmd	*format_cmds(int argc, char **argv, char **envp);
void	free_cmds(t_cmd *cmds);
void	free_setnull(int count, void **ptr, ...);
int		get_input_fd(const char *input_name);
int		get_outfile_fd(t_cmd *cmds, char *outfile);
bool	is_delimiter(char *line, char *delimiter);
bool	not_enough_args(int argc, char **argv);
void	read_from_stdin(t_cmd *cmds, int *pipes);
void	redirect_and_exec_cmd(t_cmd cmd, int stdin_dup2, int stdout_dup2,
			int *pipes);

#endif