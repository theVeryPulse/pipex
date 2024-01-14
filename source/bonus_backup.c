/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_backup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Philip <juli@student.42london.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 22:54:23 by Philip            #+#    #+#             */
/*   Updated: 2024/01/14 12:38:25 by Philip           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * @brief 
 * 
 * [x] Checks if infile exists
 * [x] Checks if all commands exist
 * [ ] Runs each command in a child process
 */

/**
 * @brief 
 * Close all the write / read ends in an array of pipes.
 * 
 * @param pipes 
 * int array storing the file descriptors of all opened pipes.
 * @param pipe_count 
 * The total amount of the pipes. Every one pipe has two ends and hence two
 * elements.
 */
void	close_all_pipes(int	*pipes, unsigned int pipe_count)
{
	unsigned int	i;

	if (!pipes)
		return ;
	i = 0;
	while (i < pipe_count * 2)
		close(pipes[i++]);
}

/**
 * @brief Check if infile exists
 * ------------------------------------------------------------------------ 
 * @param argv[1] (infile name)
 * @return infile_fd, which equals to -1 when open fails
*/
// int	infile_check()
// {
	
// }

int	main(int argc, char **argv)
{
	if (argc < 5)
	{
		ft_putendl_fd("command incomplete", STDERR_FILENO);
		return (1);
	}

/**
 * @brief Get infile fd
 * ------------------------------------------------------------------------ 
 * @param file_name (char *) argv[1]  The infile name
 * 
 * @return infile_fd (int) Returns -1 when open fails
*/
	int		infile_fd;

	if (ft_strncmp(argv[1], "here_doc", ft_strlen("here_doc") + 1) == 0)
	{
		ft_printf("here doc mode\n");
		// infile_fd = STDIN_FILENO;
	}
	else
		infile_fd = open(argv[1], O_RDONLY);
	if (infile_fd == -1)
	{
		perror(argv[1]);
		return (1);
	}

/**
 * @brief Formats and checks if commands exist and executable
 * =========================================================================
 * @param cmd_count In this case (argc - 3)
 * @param argv To format the executable path for commands
 * 
 * @return cmds(t_cmd[])
 * char ***cmds_argv:
 * {
 *   {"cmd1", "flag1", ...},
 *   {"cmd2", "flag1", ...},
 *   ...
 * }
 * 
 * char **cmds_path:
 * {
 *   {"/usr/bin/cmd1"},
 *   {"/usr/bin/cmd2"},
 *   ...
 * }
 * 
 */
	int		i;
	int		j;
	int		cmd_count;
	char	***cmds_argv;

/* Split commands names and flags ------------------------------------------- */
	cmds_argv = (char ***)ft_calloc(cmd_count + 1, sizeof(char **));
	cmd_count = argc - 3;
	if (!cmds_argv)
		return (2);
	i = 2;
	j = 0;
	while (i < argc - 1)
		cmds_argv[j++] = ft_split(argv[i++], ' ');


/* Build command program paths ---------------------------------------------- */
	char	**cmds_path;

	cmds_path = (char **)ft_calloc(cmd_count + 1, sizeof(char *));
	if (!cmds_path)
	{
		return (3);
	}

	i = 0;
	while (i < cmd_count)
	{
		cmds_path[i] = format_string("/usr/bin/%s", cmds_argv[i][0]);
		i++;
	}

/* Checking the command programs -------------------------------------------- */
	i = 0;
	while (i < cmd_count)
	{
		// ft_printf("Checking %s\n", cmds_argv[i][0]);
		if (access(cmds_path[i], F_OK | X_OK) == -1)
		{
			ft_putstr_fd(cmds_argv[i][0], STDERR_FILENO);
			ft_putendl_fd(": command not found", STDERR_FILENO);
			// free and ends main
			return (127);
		}
		i++;
	}
	
/** Pipes
 * ------------------------------------------------------------------------- 
 * @brief build pipes between each two commands, equalling to command count 
 * minus one.
 * 
 * @param pipe_count (unsigned int) In this case (cmd_count - 1) or (argc - 4)
 * 
 * @return int pipes[pipe_count * 2]
 * Each pairs are the read/write ends of one pipe.
 * E.g. pipes[0] pipes[1] for 1st pipe, pipes[2] pipes[3] for second pipe.
 */
	int	*pipes;
	
	pipes = (int *)ft_calloc(cmd_count - 1, sizeof(int) * 2);
	i = 0;
	while (i < cmd_count - 1) // 0 2 4 6 8
	{
		// ft_printf("Creating pipe at pipes[%d]\n", i * 2);
		if (pipe(&pipes[i * 2]) == -1)
		{
			// free and exit
			perror("pipex");
			return (2);
		}
		i++;
	}
	// ft_printf("pipe[0]: %d, pipe[1]: %d\n", pipes[0], pipes[1]);

	char	*env[2];

	env[0] = "PATH=/nfs/homes/juli/bin:/nfs/homes/juli/bin:/usr/local/sbin"
		":/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:"
		"/usr/local/games:/snap/bin";
	env[1] = NULL;

/**
 * First command
 * -------------------------------------------------------------------------
 * @brief Execute the first command, which reads input from given infile 
 * file descriptor.
 * 
 * @param infile_fd int
 * @param pipes int *
 * @param cmds_path char**
 * @param cmds_argv char***
 * 
 * @return status code, -1 for error
 */
	pid_t	id_first;

	id_first = fork();
	if (id_first == -1)
	{
		// free all malloc
		return (-1);
	}
	if (id_first == 0)
	{
		dup2(infile_fd , STDIN_FILENO);
		close(infile_fd);
		dup2(pipes[1], STDOUT_FILENO);
		close_all_pipes(pipes, cmd_count - 1);
		if (execve(cmds_path[0], cmds_argv[0], env) == -1)
		{
			perror(cmds_argv[0][0]);
			return (-1);
		}
	}

/**
 *  Middle commands 
 * ------------------------------------------------------------------------- 
 * @brief iterate through all the commands between first and last commands in
 * independent child processes.
 * 
 * @param cmd_count int
 * @param pipes int[]
 * @param cmds_path char**
 * @param cmds_argv char***
 * 
 * @return status code (int), -1 upon error, and 0 upon success.
 * 
 */
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
			// ft_printf("Mid: %s in %d\n", cmds_path[cmd_i], getpid());
			dup2(pipes[0 + (cmd_i - 1) * 2], STDIN_FILENO);
			dup2(pipes[1 + cmd_i * 2], STDOUT_FILENO);
			close_all_pipes(pipes, cmd_count - 1);
			if (execve(cmds_path[cmd_i], cmds_argv[cmd_i], env) == -1)
			{
				perror("Error");
				return (-1);
			}
		}
		cmd_i++;
	}
	
/**
 * Last command 
 * ------------------------------------------------------------------------- 
 * @brief Executes the last command and output to target file.
 * 
 * @param cmds_path (char**)
 * @param cmds_argv (char***)
 * @param cmd_i (size_t) In this case (argc - 4)
 * @param outfile (char*) In this case, (argv[argc - 1])
 * 
 * @return id_last(pid_t) For parent process to receive the exit code of child.
 */
	pid_t	id_last;
	int		outfile_fd;

	cmd_i = argc - 4;
	id_last = fork();
	if (id_last == -1)
		return (2);
	if (id_last == 0)
	{
		outfile_fd =  open(argv[argc - 1], O_CREAT | O_WRONLY, 0777);
		if (outfile_fd == -1)
		{
			perror("pipex");
			return (127);
		}
		dup2(outfile_fd, STDOUT_FILENO);
		close(outfile_fd);
		dup2(pipes[0 + (cmd_i - 1) * 2], STDIN_FILENO);
		close_all_pipes(pipes, cmd_count - 1);
		if (execve(cmds_path[cmd_i], cmds_argv[cmd_i], env) == -1)
		{
			// free and exit
			perror(cmds_argv[cmd_i][0]);
			return (9);
		}
	}


	close_all_pipes(pipes, cmd_count - 1);
	close(infile_fd);

	int	stat_val;

	waitpid(id_last, &stat_val, 0);
	if (WIFEXITED(stat_val))
		stat_val = WEXITSTATUS(stat_val);
	return (stat_val);
}
