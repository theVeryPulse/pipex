/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_funcs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Philip <juli@student.42london.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 22:30:02 by Philip            #+#    #+#             */
/*   Updated: 2024/01/16 22:44:40 by Philip           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	exec_commands(int argc, char **argv, char **envp)
{
	int		input_fd;
	int		*pipes;
	pid_t	id_last;
	t_cmd	*cmds;
	int		exit_stats;

	input_fd = get_input_fd(argv[1]);
	cmds = format_cmds(argc, argv, envp);
	check_cmds(cmds, argc - 3);
	pipes = build_pipes(argc - 4);
	exec_first_command(cmds, pipes, input_fd);
	exec_middle_commands(cmds, pipes, argc - 3);
	id_last = exec_last_command(cmds, pipes, argv[argc - 1]);
	close(input_fd);
	close_all_pipes(pipes);
	exit_stats = exit_status_of_last_cmd(id_last);
	free(pipes);
	free_cmds(cmds);
	exit(exit_stats);
}

void	exec_first_command(t_cmd *cmds, int *pipes, int input_fd)
{
	pid_t	id;

	id = fork();
	if (id == -1)
	{
		return ;
	}
	if (id == 0 && cmds[0].is_delimiter && input_fd == STDIN_FILENO)
	{
		read_from_stdin(cmds, pipes);
	}
	else if (id == 0 && !(cmds[0].is_delimiter) && input_fd != STDIN_FILENO)
	{
		redirect_and_exec_cmd(cmds[0],
			input_fd,
			pipes[1],
			pipes);
	}
}

/**
 * @param cmd_count (int) In this case (argc - 3)
 * @return Status code (int), '-1' upon error, and '0' upon success.
 * @note Each command reads from the read-end of the pipe between last
 * command, and writes to the write-end of the pipe between next command.
 */
void	exec_middle_commands(t_cmd *cmds, int *pipes, int cmd_count)
{
	int	cmd_i;
	int	id;

	cmd_i = 1;
	while (cmd_i < cmd_count - 1)
	{
		id = fork();
		if (id == -1)
			return ;
		if (id == 0)
		{
			redirect_and_exec_cmd(cmds[cmd_i],
				pipes[0 + (cmd_i - 1) * 2],
				pipes[1 + cmd_i * 2],
				pipes);
		}
		cmd_i++;
	}
}

pid_t	exec_last_command(t_cmd *cmds, int *pipes, char *outfile)
{
	int		last_i;
	pid_t	id;
	int		outfile_fd;

	last_i = 0;
	while (cmds[last_i + 1].argv)
		last_i++;
	id = fork();
	if (id == -1)
		return (-1);
	if (id == 0)
	{
		outfile_fd = get_outfile_fd(cmds, outfile);
		if (outfile_fd == -1)
		{
			perror(outfile);
			exit (NO_PERMIT);
		}
		redirect_and_exec_cmd(cmds[last_i],
			pipes[0 + (last_i - 1) * 2],
			outfile_fd,
			pipes);
	}
	return (id);
}

void	redirect_and_exec_cmd(t_cmd cmd, int stdin_dup2, int stdout_dup2,
				int *pipes)
{
	dup2(stdin_dup2, STDIN_FILENO);
	dup2(stdout_dup2, STDOUT_FILENO);
	close(stdin_dup2);
	close(stdout_dup2);
	close_all_pipes(pipes);
	if (execve(cmd.path, cmd.argv, cmd.envp) == -1)
	{
		perror(cmd.argv[0]);
		exit (127);
	}
	exit(0);
}
