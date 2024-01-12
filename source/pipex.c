/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Philip <juli@student.42london.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 15:28:54 by Philip            #+#    #+#             */
/*   Updated: 2024/01/12 19:27:32 by Philip           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// $ ./pipex infile "ls -l" "wc -l" outfile
// follows 
// $ < file1 ls -l | wc -l > file2

// $ ./pipex file1 cmd1 cmd2 cmd3 ... cmdn file2
// follows
// $ < file1 cmd1 | cmd2 | cmd3 ... | cmdn > file2

// ./pipex here_doc END cmd cmd1 file
// follows
// $ cmd <<END | cmd1 >> file

// PATH=/nfs/homes/juli/bin:/nfs/homes/juli/bin:/usr/local/sbin:/usr/local/bin
// :/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin

void	err_msg(const char *s)
{
	write(STDERR_FILENO, s, ft_strlen(s));
}

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

	/* Check if infile exists-------------------------------------------------*/
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

	/* Check if first command exists------------------------------------------*/
	char	*cmd1_bin_path;
	
	cmd1 = ft_split(argv[2], ' ');
	// for (int i = 0; cmd1[i]; i++) ft_printf("argv[%d]: %s\n", i, cmd1[i]);
	// ft_printf("Checking if \"%s\" is executable: ", cmd1[0]);
	cmd1_bin_path = format_string("/usr/bin/%s", cmd1[0]);
	if (access(cmd1_bin_path, F_OK | X_OK) == -1) //   /usr/bin/grep
	{
		ft_printf("pipex: %s: ", cmd1[0]);
		perror("");
	}

	/* Check if second command exists --------------------------------------- */
	char	*cmd2_bin_path;

	cmd2 = ft_split(argv[3], ' ');
	// for (int i = 0; cmd2[i]; i++) ft_printf("argv[%d]: %s\n", i, cmd2[i]);
	// ft_printf("Checking if \"%s\" is executable: ", cmd2[0]);
	cmd2_bin_path = format_string("/usr/bin/%s", cmd2[0]);
	if (access(cmd2_bin_path, F_OK | X_OK) == -1) //   /usr/bin/grep
	{
		ft_printf("pipex: %s: ", cmd1[0]);
		perror("");
	}
	
	/* Pipe between processes ================================================*/
	int		my_pipe[2];

	if (pipe(my_pipe) == -1)
		return (2);

	char	*env[2];

	env[0] = "PATH=/nfs/homes/juli/bin:/nfs/homes/juli/bin:/usr/local/sbin"
	":/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games"
	":/snap/bin";
	env[1] = NULL;
	/* First command =========================================================*/
	pid_t	id1;

	id1 = fork();
	if (id1 == -1)
		return (1);
	if (id1 == 0)
	{
		close(my_pipe[0]);

		// ft_printf("Executing '%s'...\n", cmd1_bin_path);
		dup2(infile_fd , STDIN_FILENO);
		dup2(my_pipe[1], STDOUT_FILENO);
		if (execve(cmd1_bin_path, cmd1, env) == -1)
		{
			return (9);
			perror("");
		}
		for (int i = 0; cmd1[i]; i++) free(cmd1[i]);
	}

	/* Last command ======================================================= */
	pid_t	id2;
	int		outfile_fd;

	id2 = fork();
	if (id2 == -1)
		return (2);
	if (id2 == 0)
	{
		close(my_pipe[1]);
		outfile_fd =  open(argv[4], O_CREAT | O_WRONLY, 0777);
		if (outfile_fd == -1)
			return (3);
		// ft_printf("Executing '%s'...\n", cmd2_bin_path);
		dup2(my_pipe[0], STDIN_FILENO);
		dup2(outfile_fd, STDOUT_FILENO);
		//  int execve(const char *path, char *const argv[], char *const envp[]);
		if (execve(cmd2_bin_path, cmd2, env) == -1)
		{
			perror("");
			return (9);
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
