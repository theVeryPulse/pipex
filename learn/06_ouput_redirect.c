/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   06_ouput_redirect.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Philip <juli@student.42london.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 00:10:39 by Philip            #+#    #+#             */
/*   Updated: 2024/01/10 12:56:04 by Philip           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Redirecting standard output in C
// https://www.youtube.com/watch?v=5fnVr-zH-SE

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <wait.h>
#include <fcntl.h>
#include <stdbool.h>

int	main(void)
{
	pid_t	id;
	int		rtn;

	printf("Building child process...\n");
	id = fork();
	if (id == -1)
	{
		printf("Fork error.\n");
		return (1);
	}

	if (id == 0)
	{
		printf("Executing child process...\n\n");

		int file = open("results.txt", O_WRONLY | O_CREAT, 0777);
		// dup2(new_fd, old_fd_to_be_closed)
		if (dup2(file, STDOUT_FILENO) == -1)
		{
			printf("Cannot redirect output.\n");
			return (2);
		}
		close(file);

		char *cmd[] = {"ls", "-l", (char *)0};
		char *env[] = {"HOME=/usr/home", "LOGNAME=home", (char *)0};
		rtn = execve("/bin/s", cmd, env);
		// Two cases when execve fails
		// - target program does not exist, execve then returns -1
		// - target program fails and returns a value other than 0, 
		if (rtn == -1)
			printf("command not found\n");
	}
	else
	{
		int	w_status;
		int	status_code;

		wait(&w_status); // Receives the exit code of child process
		if (WIFEXITED(w_status) == true)
			status_code = WEXITSTATUS(w_status);
		if (status_code == 0 && rtn != -1)
			printf("\nChild process sucess.\n");
		else
			printf("Child process exited with code %d.\n", status_code);
	}
}