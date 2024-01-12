/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Philip <juli@student.42london.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 22:54:23 by Philip            #+#    #+#             */
/*   Updated: 2024/01/12 23:05:21 by Philip           ###   ########.fr       */
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

void	err_msg(const char *s)
{
	write(STDERR_FILENO, s, ft_strlen(s));
}

int	main(int argc, char **argv)
{
	pid_t	id;

	if (argc < 5)
	{
		// err_msg("command incomplete\n");
		return (1);
	}

	/* Check if infile exists------------------------------------------------ */
	int		infile_fd;
	char	*str;

	if (ft_strncmp(argv[1], "here_doc", ft_strlen("here_doc") + 1) == 0)
	{
		infile_fd = STDIN_FILENO;
	}
	else
	{
		infile_fd = open(argv[1], O_RDONLY);
	}
	if (infile_fd == -1)
	{
		str = format_string("pipex: %s: ", argv[1]);
		write(STDERR_FILENO, str, ft_strlen(str));
		perror("");
		return (1);
	}

	/* Check if commands exist ------------------------------------------*/
	// $ ./pipex file1 cmd1 cmd2 cmd3 ... cmdn file2
	// follows
	// $ < file1 cmd1 | cmd2 | cmd3 ... | cmdn > file2
	// number of cmds: argc - 3
	// ./pipex infile cmd1 cmd2 cmd3 outfile
	// 0       1      2    3    4    5
	// argc 6, 
	int	i;
	int	j;
	char	***cmds;
	int		cmd_count;

	// ft_printf("Checking the commands...\n");
	/* Split commands names and flags */
	cmd_count = argc - 3;
	cmds = (char ***)ft_calloc(cmd_count + 1, sizeof(char **));
	if (!cmds)
		return (2);
	i = 2;
	j = 0;
	while (i < argc - 1)
		cmds[j++] = ft_split(argv[i++], ' ');

	/* Build command program paths */
	char	**cmds_paths;
	cmds_paths = (char **)ft_calloc(cmd_count + 1, sizeof(char *));
	if (!cmds_paths)
	{
		// free
		return (3);
	}
	i = 0;
	while (i < cmd_count)
	{
		cmds_paths[i] = format_string("/usr/bin/%s", cmds[i][0]);
		i++;
	}
	i = 0;
	/* while (cmds_paths[i])
	{
		ft_printf("cmd[%d] path: %s\n", i + 1, cmds_paths[i]);
		i++;
	} */

	/* Checking the command programs */
	i = 0;
	while (i < cmd_count)
	{
		// ft_printf("Checking %s\n", cmds[i][0]);
		if (access(cmds_paths[i], F_OK | X_OK) == -1)
		{
			// ft_printf("pipex: %s: ", cmds[i][0]);
			perror("Error");
			// free and ends main
		}
		i++;
	}
	
	/* Pipes ================================================================*/
	int *pipes = (int *)ft_calloc(cmd_count - 1, sizeof(int) * 2);
	i = 0;
	while (i < cmd_count - 1) // 0 2 4 6 8
	{
		ft_printf("Creating pipe at pipes[%d]\n", i * 2);
		if (pipe(&pipes[i * 2]) == -1)
		{
			// free and exit
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

	/* First command =========================================================*/
	pid_t	id_first;

	id_first = fork();
	if (id_first == -1)
		return (1);
	if (id_first == 0)
	{
		ft_printf("Executing '%s' in %d...\n", cmds[0][0], getpid());
		ft_printf("pipe write fd: %d, read fd: %d\n", pipes[1], pipes[0]);
		ft_printf("%d pipe write fd: %d, read fd: %d\n",
					__LINE__, pipes[1], pipes[0]);

		close(pipes[0]);
		dup2(infile_fd , STDIN_FILENO);
		close(infile_fd);
		dup2(pipes[1], STDOUT_FILENO);
		close(pipes[1]);
		if (execve(cmds_paths[0], cmds[0], env) == -1)
		{
			err_msg("First command error\n");
			perror("Error");
			return (9);
		}
		// free and exit
	}

	/* Middle commands ====================================================== */
	/** 
	 * cmd1 < infile pipes[1] index:0
	 * cmd2 pipes[0] pipes[3] index:1
	 * cmd3 pipes[2] pipes[5] index:2 write to pipes[1 + cmd_i * 2]
	 * cmd4 pipes[4] >outfile index:3 read from pipes[0+ (cmd_i - 1) * 2]
	 * 
	 * cmds[0]
	 * [0] 0 1
	 * cmds[1] read ((cmd_i - 1) * 2) | write ((cmd_i * 2 + 1)
	 * [1] 2 3
	 * cmds[2]
	 * [2] 4 5
	 * cmds[3]
	 * [3] 6 7
	 * cmds[4]
	 * [4] 8 9
	 * cmds[5]
	 * outfd
	 * */
	int	cmd_i;
	cmd_i = 1;
	while (cmd_i < cmd_count - 1)
	{
		int	id;
		int	i;
		id = fork();
		if (id == -1)
			; // free and exit
		if (id == 0)
		{
			ft_printf("Mid: %s in %d\n", cmds_paths[cmd_i], getpid());
			dup2(pipes[0 + (cmd_i - 1) * 2], STDIN_FILENO);
			dup2(pipes[1 + cmd_i * 2], STDOUT_FILENO);

			i = 0;
			while (i < cmd_count - 1)
				close(pipes[i++]);

			if (execve(cmds_paths[cmd_i], cmds[cmd_i], env) == -1)
			{
				perror("Error");
				return (9);
			}
		}
		cmd_i++;
	}
	
	/* Last command ========================================================= */
	pid_t	id_last;
	int		outfile_fd;

	id_last = fork();
	if (id_last == -1)
		return (2);
	if (id_last == 0)
	{
		ft_printf("%d\n", __LINE__);
		ft_printf("Executing '%s' in %d\n", cmds[cmd_i][0], getpid());


		outfile_fd =  open(argv[argc - 1], O_CREAT | O_WRONLY, 0777);
		if (outfile_fd == -1)
		{
			perror("pipex");
			return (127);
		}
		ft_printf("%d pipe outifle fd: %d, read fd: %d\n",
					__LINE__, pipes[1 + cmd_i * 2], pipes[0 + (cmd_i - 1) * 2]);

		// close(pipes[1 + cmd_i * 2]);
		ft_printf("Opened file %s\n", argv[argc - 1]);
		// pipe[0]: 3, STDIN: 0->terminal
		// fd pipe[0]: 3 -> pipe-read-end
		// fd STDIN:   0 -> pipe-read-end
		// close(pipes[0 + (cmd_i - 1) * 2]);
		dup2(outfile_fd, STDOUT_FILENO);
		close(outfile_fd);
		dup2(pipes[0 + (cmd_i - 1) * 2], STDIN_FILENO);
		i = 0;
		while (i < (cmd_count - 1) * 2)
			close(pipes[i++]);
		close(pipes[0]);
		if (execve(cmds_paths[cmd_i], cmds[cmd_i], env) == -1)
		{
			err_msg("Last command error\n");
			// free and exit
			perror("Error");
			return (9);
		}
	}
	i = 0;
	while (i < (cmd_count - 1) * 2)
		close(pipes[i++]);
	close(infile_fd);
	int	stat_val;
	waitpid(id_last, &stat_val, 0);
	if (WIFEXITED(stat_val))
		stat_val = WEXITSTATUS(stat_val);
	return (stat_val);
}
