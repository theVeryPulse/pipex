/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   05_01_exec_in_fork.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Philip <juli@student.42london.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 22:11:31 by Philip            #+#    #+#             */
/*   Updated: 2024/01/09 22:31:20 by Philip           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Executing commands in C
// https://www.youtube.com/watch?v=uh2FqejQRk8

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
		int ret;
		char *cmd[] = {"ls", "-l", (char *)0};
		char *env[] = {"HOME=/usr/home", "LOGNAME=home", (char *)0};
		ret = execve("/bin/ls", cmd, env);
		printf("Other program execution failed.\n");
	}
	else
	{
		wait(NULL);
		printf("\nChild process compelete.\n");
	}
}