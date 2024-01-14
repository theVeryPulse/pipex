/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Philip <juli@student.42london.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 22:54:23 by Philip            #+#    #+#             */
/*   Updated: 2024/01/14 18:04:59 by Philip           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * @brief Close all the write / read ends in an array of pipes.
 * @param pipes (int) array storing the file descriptors of all opened pipes.
 * @param pipe_count (int) The total amount of the pipes.
 * @note Every one pipe has two ends, all ends need closing.
 */
// void	close_all_pipes(int	*pipes, int pipe_count)
void	close_all_pipes(int	*pipes)
{
	int	i;

	if (!pipes)
		return ;
	i = 0;
	// while (i < pipe_count * 2)
		// close(pipes[i++]);
	while (pipes[i])
		close(pipes[i++]);
}

/**
 * @brief Check if infile exists
 * @param argv[1] (infile name)
 * @return infile_fd, which equals to -1 when open fails
*/
int	get_infile_fd(const char *infile_name)
{
	int	infile_fd;

	if (ft_strncmp(infile_name, "here_doc", ft_strlen("here_doc") + 1) == 0)
	{
		infile_fd = STDIN_FILENO;
		ft_printf("here doc mode\n");
	}
	else
	{
		infile_fd = open(infile_name, O_RDONLY);
	}
	if (infile_fd == -1)
	{
		perror(infile_name);
	}
	return (infile_fd);
}

/**
 * @brief Formats commands' paths and argvs
 * @param cmd_count In this case (argc - 3)
 * @param argv To format the executable path for commands
 * @return cmds(t_cmd[]) NULL terminated.
 */
t_cmd	*format_cmds(int argc, char **argv)
{
	int		arg_i;
	int		cmd_i;
	t_cmd	*cmds;
	int		cmd_count;

	cmd_count = argc - 3;
	cmds = (t_cmd *)ft_calloc(cmd_count + 1, sizeof(t_cmd));
	if (!cmds)
		return (NULL);
	arg_i = 2;
	cmd_i = 0;
	while (arg_i < argc - 1)
		cmds[cmd_i++].argv = ft_split(argv[arg_i++], ' ');
	cmd_i = 0;
	while (cmd_i < cmd_count)
	{
		cmds[cmd_i].path = format_string("/usr/bin/%s", cmds[cmd_i].argv[0]);
		cmd_i++;
	}
	return (cmds);
}

int	check_cmds(t_cmd *cmds, int cmd_count)
{
	int	i;

	i = 0;
	while (i < cmd_count)
	{
		if (access(cmds[i].path, F_OK | X_OK) == -1)
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
 * @brief Builds one pipe between each two commands
 * @param pipe_count (int) In this case (cmd_count - 1) or (argc - 4)
 * @return int pipes[pipe_count * 2]. Array is NULL terminated.
 * @note Every one pipe has two ends (file descriptors), all ends need closing.
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

char	**my_env(t_mode mode)
{
	static char	**env;

	if (mode == GET && !env)
	{
		env = (char **)ft_calloc(2, sizeof(char *));
		env[0] = "PATH=/nfs/homes/juli/bin:/nfs/homes/juli/bin:/usr/local/sbin"
			":/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:"
			"/usr/local/games:/snap/bin";
	}
	else if (mode == FREE && env)
	{
		free(env);
		env = NULL;
	}
	return (env);
}

/**
 * @brief Input is redirected to infile_fd, output is redirected to pipes[1].
 * @param infile_fd Input for the first command
 * @return status code, -1 for error
 */
int	exec_first_command(t_cmd *cmds, int infile_fd, int* pipes)
{
	pid_t	id_first;
	
	id_first = fork();
	if (id_first == -1)
	{
		return (-1);
	}
	if (id_first == 0)
	{
		ft_printf("line %d\n", __LINE__);

		dup2(infile_fd , STDIN_FILENO);
		close(infile_fd);
		dup2(pipes[1], STDOUT_FILENO);
		close_all_pipes(pipes);
		if (execve(cmds[0].path, cmds[0].argv, my_env(GET)) == -1)
		{
			perror(cmds[0].argv[0]);
			return (-1);
		}
	}
	return (0);
}

/**
 * @brief Iterates through each command between first and last commands in
 * independent child processes.
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
			if (execve(cmds[cmd_i].path, cmds[cmd_i].argv, my_env(GET)) == -1)
			{
				perror("Error");
				return (-1);
			}
		}
		cmd_i++;
	}
	return (0);
}

/**
 * @param outfile The output file, in this case argv[argc - 1]
 * @return Process ID of the last command (pid_t), so that the last command can
 * be tracked by the parent process and capture its return value.
 */
pid_t	exec_last_command(t_cmd *cmds, int *pipes, char *outfile)
{
	int		last_i;
	pid_t	id_last;
	int		outfile_fd;

	last_i = 0;
	while (cmds[last_i + 1].argv)
		last_i++;

	id_last = fork();
	if (id_last == -1)
		return (2);
	if (id_last == 0)
	{
		outfile_fd =  open(outfile, O_CREAT | O_WRONLY, 0777);
		if (outfile_fd == -1)
		{
			perror("pipex");
			return (127);
		}
		dup2(outfile_fd, STDOUT_FILENO);
		close(outfile_fd);
		dup2(pipes[0 + (last_i - 1) * 2], STDIN_FILENO);
		close_all_pipes(pipes);
		if (execve(cmds[last_i].path, cmds[last_i].argv, my_env(GET)) == -1)
		{
			perror(cmds[last_i].argv[0]);
			return (9);
		}
	}
	return (id_last);
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

// int	exec_commands_heredoc(int argc, char **argv)
// {
// 	char	*line;
// 	char	*delimiter;
// 	pid_t	gnl_id;
// 	int		heredoc_pipe[2];

// 	if (argc < 6)
// 	{
// 		ft_putendl_fd("command incomplete", STDERR_FILENO);
// 		exit (1);
// 	}
// 	if (pipe(heredoc_pipe) == -1)
// 		exit (1);

// 	delimiter = format_string("%s\n", argv[2]);
// 	if (!delimiter)
// 		exit (1);

// 	gnl_id = fork();
// 	if (gnl_id == -1)
// 		return (1);

// 	if (gnl_id == 0)
// 	{
// 		close(heredoc_pipe[0]);
// 		while (true)
// 		{
// 			line = get_next_line(STDIN_FILENO);
// 			if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
// 				break;
// 			free_setnull(1, (void **)&line);
// 		}
// 		free_setnull(2, (void **)&line, (void **)&delimiter);
// 		close(heredoc_pipe[1]);
// 		exit (0);
// 	}
// 	close(heredoc_pipe[0]);
// 	close(heredoc_pipe[1]);
// 	exit (0);
// }

int	main(int argc, char **argv)
{
	int		infile_fd;
	t_cmd	*cmds;
	int		*pipes;
	pid_t	id_last;

	if (argc < 5)
	{
		ft_putendl_fd("command incomplete", STDERR_FILENO);
		return (1);
	}
	cmds =  format_cmds(argc, argv);
	if (check_cmds(cmds, argc - 3) == 127)
		return (127);
	// if (ft_strncmp("here_doc", argv[1], ft_strlen("here_doc") + 1) == 0)
	// {
	// 	exec_commands_heredoc(argc, argc);
	// }
	infile_fd = get_infile_fd(argv[1]);
	pipes = build_pipes(argc - 4);
	if (!pipes)
		return (1);
	exec_first_command(cmds, infile_fd, pipes);
	exec_middle_commands(cmds, pipes, argc - 3);
	id_last = exec_last_command(cmds, pipes, argv[argc - 1]);

	close_all_pipes(pipes);
	close(infile_fd);
	return (exit_status_of_last_cmd(id_last));
}
