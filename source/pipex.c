/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Philip <juli@student.42london.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 22:54:23 by Philip            #+#    #+#             */
/*   Updated: 2024/01/16 20:18:39 by Philip           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * @note Function ends upon value zero, so STDIN_FILENO should not be inside the
 * array.
 */
void	close_all_pipes(int	*pipes)
{
	int	i;

	if (!pipes)
		return ;
	i = 0;
	while (pipes[i])
		close(pipes[i++]);
}
/**
 * @return File descriptor of input file, or 0 (STDIN_FILENO) when infile name
 * is "here_doc".
 * Returns -1 when open fails
*/
int	get_input_fd(const char *input_name)
{
	int	infile_fd;

	if (ft_strncmp(input_name, "here_doc", ft_strlen("here_doc") + 1) == 0)
	{
		infile_fd = STDIN_FILENO;
		ft_printf("here doc mode\n");
	}
	else
	{
		infile_fd = open(input_name, O_RDONLY);
	}
	if (infile_fd == -1)
	{
		perror(input_name);
	}
	return (infile_fd);
}

/**
 * @return cmds(t_cmd[]) NULL terminated.
 */
t_cmd	*format_cmds(int argc, char **argv, char **envp)
{
	int		i;
	t_cmd	*cmds;
	int		cmd_count;
	char	**cmd_start;

	cmd_count = argc - 3;
	cmd_start = argv + 2;
	cmds = (t_cmd *)ft_calloc(cmd_count + 1, sizeof(t_cmd));
	if (!cmds)
		return (NULL);
	i = 0;
	while (i < cmd_count)
	{
		cmds[i].argv = ft_split(cmd_start[i], ' ');
		cmds[i].path = format_string("/usr/bin/%s", cmds[i].argv[0]);
		cmds[i].envp = envp;
		cmds[i].is_delimiter = false;
		i++;
	}
	if (ft_strncmp(argv[1], "here_doc", ft_strlen("here_doc") + 1) == 0)
		cmds[0].is_delimiter = true;
	return (cmds);
}

void	free_cmds(t_cmd *cmds)
{
	int	i;
	int	j;

	i = 0;
	while (cmds[i].argv)
	{
		j = 0;
		while (cmds[i].argv[j])
			free(cmds[i].argv[j++]);
		free(cmds[i].argv);
		free(cmds[i].path);
		i++;
	}
	free(cmds);
}

int	check_cmds(t_cmd *cmds, int cmd_count)
{
	int	i;

	i = 0;
	while (i < cmd_count)
	{
		if (!(cmds[i].is_delimiter) && access(cmds[i].path, F_OK | X_OK) == -1)
		{
			ft_putendl_fd(cmds[i].path, STDERR_FILENO);
			ft_putstr_fd(cmds[i].argv[0], STDERR_FILENO);
			ft_putendl_fd(": command not found", STDERR_FILENO);
			return (127);
		}
		i++;
	}
	return (0);
}

/**
 * @param pipe_count (int) In this case (cmd_count - 1) or (argc - 4)
 * @return int pipes[pipe_count * 2]. Array is 0 terminated.
 */
int	*build_pipes(int pipe_count)
{
	int	i;
	int	*pipes;
	
	pipes = (int *)ft_calloc(pipe_count + 1, sizeof(int) * 2);
	i = 0;
	while (i < pipe_count)
	{
		if (pipe(&pipes[i * 2]) == -1)
		{
			perror("pipex");
			return (NULL);
		}
		i++;
	}
	return (pipes);
}

int	exit_status_of_last_cmd(pid_t id_last)
{
	int	stat_val;

	waitpid(id_last, &stat_val, 0);
	if (WIFEXITED(stat_val))
		stat_val = WEXITSTATUS(stat_val);
	return (stat_val);
}

void	free_setnull(int count, void **ptr, ...)
{
	int		i;
	va_list	ap;
	void	**this;

	va_start(ap, ptr);
	if (ptr || *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
	i = 1;
	while (i < count)
	{
		this = va_arg(ap, void **);
		if (!this || *this)
			continue ;
		free(*this);
		*this = NULL;
	}
}

bool	is_delimiter(char *line, char *delimiter)
{
	size_t	i;

	if (!line || !delimiter)
		return (false);
	i = 0;
	while (delimiter[i])
	{
		if (line[i] != delimiter[i])
		{
			return (false);
		}
		i++;
	}
	return (line[i] == '\n' && line[i + 1] == '\0');
}

int	read_from_stdin(t_cmd *cmds, int *pipes)
{
	char *line;

	while (1)
	{
		line = get_next_line(STDIN_FILENO);
		if (is_delimiter(line, cmds[0].argv[0]))
			break;
		if (line)
		{
			write(pipes[1], line, ft_strlen(line));
		}
		free_setnull(1, (void **)&line);
	}
	free_setnull(1, (void **)&line);
	close_all_pipes(pipes);
	exit (0);
}

int	redirect_and_exec_cmd(t_cmd cmd, int input_fd )
{
	;
}

int	exec_first_command(t_cmd *cmds, int *pipes, int input_fd)
{
	pid_t	id;
	// char	*line;

	id = fork();
	if (id == -1)
	{
		return (-1);
	}
	if (id == 0 && cmds[0].is_delimiter && input_fd == STDIN_FILENO)
	{
		read_from_stdin(cmds, pipes);
	}
	else if (id ==0 && !(cmds[0].is_delimiter) && input_fd != STDIN_FILENO)
	{
		dup2(input_fd , STDIN_FILENO);
		close(input_fd);
		dup2(pipes[1], STDOUT_FILENO);
		close_all_pipes(pipes);
		if (execve(cmds[0].path, cmds[0].argv, cmds[0].envp) == -1)
		{
			perror(cmds[0].argv[0]);
			exit (127);
		}
	}
	return (0);
}

/**
 * @param cmd_count (int) In this case (argc - 3)
 * @return Status code (int), '-1' upon error, and '0' upon success.
 * @note Each command reads from the read-end of the pipe between last
 * command, and writes to the write-end of the pipe between next command.
 */
int	exec_middle_commands(t_cmd *cmds, int *pipes, int cmd_count)
{
	int	cmd_i;

	cmd_i = 1;
	while (cmd_i < cmd_count - 1)
	{
		int	id;
		id = fork();
		if (id == -1)
			return (-1);
		if (id == 0)
		{
			dup2(pipes[0 + (cmd_i - 1) * 2], STDIN_FILENO);
			dup2(pipes[1 + cmd_i * 2], STDOUT_FILENO);
			close_all_pipes(pipes);
			if (execve(cmds[cmd_i].path, cmds[cmd_i].argv, cmds[cmd_i].envp) == -1)
			{
				perror(cmds[cmd_i].argv[0]);
				exit (127);
			}
		}
		cmd_i++;
	}
	return (0);
}

int	get_outfile_fd(t_cmd *cmds, char *outfile)
{
	int	outfile_fd;

	if (cmds[0].is_delimiter)
		outfile_fd = open(outfile, O_CREAT | O_WRONLY | O_APPEND, 0777);
	else
		outfile_fd = open(outfile, O_CREAT | O_WRONLY | O_TRUNC, 0777);
	return (outfile_fd);
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
			perror("pipex");
			exit (127);
		}
		dup2(outfile_fd, STDOUT_FILENO);
		close(outfile_fd);
		dup2(pipes[0 + (last_i - 1) * 2], STDIN_FILENO);
		close_all_pipes(pipes);
		if (execve(cmds[last_i].path, cmds[last_i].argv, cmds[last_i].envp) == -1)
		{
			perror(cmds[last_i].argv[0]);
			exit (127);
		}
	}
	return (id);
}

int	exec_commands(int argc, char **argv, char **envp)
{
	int		input_fd;
	int		*pipes;
	pid_t	id_last;
	t_cmd	*cmds;
	int		exit_stats;

	cmds = format_cmds(argc, argv, envp);
	if (!cmds)
		exit (1);
	if (check_cmds(cmds, argc - 3) == 127)
		exit (127);
	pipes = build_pipes(argc - 4);
	if (cmds[0].is_delimiter)
		input_fd = STDIN_FILENO;
	else
		input_fd = get_input_fd(argv[1]);
	exec_first_command(cmds, pipes, input_fd);
	exec_middle_commands(cmds, pipes, argc - 3);
	id_last = exec_last_command(cmds, pipes, argv[argc - 1]);
	close_all_pipes(pipes);
	if (input_fd != STDIN_FILENO)
		close(input_fd);
	exit_stats = exit_status_of_last_cmd(id_last);
	free(pipes);
	free_cmds(cmds);
	exit(exit_stats);
}

bool	not_enough_args(int argc, char **argv)
{
	if (ft_strncmp(argv[1], "here_doc", ft_strlen("here_doc") + 1) == 0
		&& argc < 6)
	{
		return (true);
	}
	if (argc < 5)
	{
		return (true);
	}
	return (false);
}

int	main(int argc, char **argv, char **envp)
{
	if (not_enough_args(argc, argv))
	{
		ft_putendl_fd("command incomplete", STDERR_FILENO);
		return (1);
	}
	exec_commands(argc, argv, envp);
}
