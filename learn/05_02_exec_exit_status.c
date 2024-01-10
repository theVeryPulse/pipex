/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   05_02_exec_exit_status.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Philip <juli@student.42london.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 22:11:31 by Philip            #+#    #+#             */
/*   Updated: 2024/01/10 00:02:04 by Philip           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Getting exit status code in C
// https://www.youtube.com/watch?v=DiNmwwQWl0g

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <wait.h>

int	main(void)
{
	pid_t	id;

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
		char *cmd[] = {"ls", "-l", (char *)0};
		char *env[] = {"HOME=/usr/home", "LOGNAME=home", (char *)0};
		int rtn = execve("/bin/ls", cmd, env);
		if (rtn == -1)
			printf("command not found\n");
	}
	else
	{
		int	w_status;
		int	status_code;

		wait(&w_status); // Receives the exit code of child process
		if (WIFEXITED(w_status))
			status_code = WEXITSTATUS(w_status);
		if (status_code == 0)
			printf("\nChild process sucess.\n");
		else
			printf("Child process exited with code %d.\n", status_code);
	}
}