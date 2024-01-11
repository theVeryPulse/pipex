/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Philip <juli@student.42london.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 22:54:23 by Philip            #+#    #+#             */
/*   Updated: 2024/01/11 23:13:55 by Philip           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv)
{
	int		fildes[2];
	pid_t	id;
	char	**cmd1;
	char	**cmd2;

	if (argc < 5)
	{
		err_msg("command incomplete\n");
		return (1);
	}

	/* Check if infile exists------------------------------------------------ */
	int		infile_fd;
	char	*str;

	infile_fd = open(argv[1], O_RDONLY);
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
	int	i;
	char	***cmds;
	int		cmd_count;
	// TODO
	cmd_count = argc - 2;
	i = 2;
	while (i < cmd_count)
	{
		char	*cmd_bin_path;
		char	**cmd;

		cmd = ft_split(argv[i], ' ');
		cmd_bin_path = format_string("/usr/bin/%s", cmd[0]);
		if (access(cmd_bin_path, F_OK | X_OK) == -1)
		{
			ft_printf("pipex: %s: ", cmd[0]);
			perror("");
		}
	}

	/* Pipe between processes ================================================*/
	int		my_pipe[2];

	if (pipe(my_pipe) == -1)
		return (2);

	/* First command =========================================================*/
	char	*env1[2];
	int		result;
	pid_t	id1;

	id1 = fork();
	if (id1 == -1)
		return (1);
	if (id1 == 0)
	{
		close(my_pipe[0]);
		env1[0] = "PATH=/nfs/homes/juli/bin:/nfs/homes/juli/bin:/usr/local/sbin"
		":/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games"
		":/snap/bin";
		env1[1] = NULL;

		ft_printf("Executing '%s'...\n", cmd1_bin_path);
		dup2(infile_fd , STDIN_FILENO);
		dup2(my_pipe[1], STDOUT_FILENO);
		//  int execve(const char *path, char *const argv[], char *const envp[]);
		if (execve(cmd1_bin_path, cmd1, env1) == -1)
		{
			return (9);
			perror("");
		}
		for (int i = 0; cmd1[i]; i++) free(cmd1[i]);
	}

	/* Last command ======================================================= */
	char	*env2[2];
	int		result2;
	pid_t	id2;
	int		outfile_fd;

	id2 = fork();
	if (id2 == -1)
		return (2);
	if (id2 == 0)
	{
		close(my_pipe[1]);
		env2[0] = "PATH=/nfs/homes/juli/bin:/nfs/homes/juli/bin:/usr/local/sbin"
		":/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games"
		":/snap/bin";
		env2[1] = NULL;
		outfile_fd =  open(argv[4], O_CREAT | O_WRONLY, 0777);
		if (outfile_fd == -1)
			return (3);
		ft_printf("Executing '%s'...\n", cmd2_bin_path);
		dup2(my_pipe[0], STDIN_FILENO);
		dup2(my_pipe[1], STDOUT_FILENO);
		//  int execve(const char *path, char *const argv[], char *const envp[]);
		if (execve(cmd2_bin_path, cmd2, env2) == -1)
		{
			return (9);
			perror("");
		}
		for (int i = 0; cmd2[i]; i++) free(cmd2[i]);
	}

	waitpid(id1, NULL, 0);
	close(my_pipe[1]);
	waitpid(id2, NULL, 0);
	close(my_pipe[0]);
	for (int i = 0; cmd1[i]; i++) free(cmd1[i]);
	for (int i = 0; cmd2[i]; i++) free(cmd2[i]);
	free(cmd1_bin_path);
	free(cmd2_bin_path);
}