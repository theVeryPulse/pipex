/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   08_00_pipe_simu.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Philip <juli@student.42london.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 16:49:29 by Philip            #+#    #+#             */
/*   Updated: 2024/01/10 19:12:57 by Philip           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <wait.h>
// PATH=/nfs/homes/juli/bin:/nfs/homes/juli/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin

int	main(int argc, char **argv, char **env)
{
	int		fildes[2];
	pid_t	id;
	pid_t	id2;

	if (pipe(fildes) == -1)
		return (1);
	/* for (int i = 0; env[i]; i++)
		printf("%d: %s\n", i, env[i]); */
	/* Child 1 for ls */
	id = fork();
	if (id == -1)
		return (2);
	if (id == 0)
	{
		int		ret;
		char	*cmd[] = {"ls", "-l", NULL};
		char	*env[] = {"HOME=/usr/home", "LOGNAME=home", NULL};

		printf("Executing child process for ls...\n");

		close(fildes[0]);
		dup2(fildes[1], STDOUT_FILENO);
		close(fildes[1]);

		ret = execve("/bin/ls", cmd, env);
		if (ret == -1)
			printf("Command not found\n");
		return (3);
	}

	/* Child 2 for grep */
	
	id2 = fork();
	if (id2 == -1)
		return (4);
	if (id2 == 0)
	{
		int		ret;
		char	*cmd[] = {"grep", "00", NULL};
		char	*env[] = {"HOME=/usr/home", "LOGNAME=home", NULL};

		printf("Executing child process for grep...\n");

		close(fildes[1]);
		dup2(fildes[0], STDIN_FILENO);
		close(fildes[0]);

		ret = execve("/bin/grep", cmd, env);
		if (ret == -1)
			printf("Command not found\n");
		return (3);
	}
	printf("Parent: Waiting for processes.\n");
	waitpid(id, NULL, 0);

	/* Note
	   When there are still open write-end file descriptors left open for grep,
	   it will keep waiting for input. Hence all write fildes need to be closed.
	 */
	close(fildes[0]);
	close(fildes[1]);

	waitpid(id2, NULL, 0);
	printf("Parent: Both processes completed.\n");
	/* {
		int	wait_status;
		int	exit_code;

		close(fildes[1]);
		wait(&wait_status);
		if (WIFEXITED(wait_status))
			exit_code = WEXITSTATUS(wait_status);
		if (exit_code == 0)
			printf("Child process compelete.\n");
		else
			printf("Process exited with code [%d].\n", exit_code);
	} */
}
