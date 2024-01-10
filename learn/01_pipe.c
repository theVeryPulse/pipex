/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juli <juli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 18:33:01 by juli              #+#    #+#             */
/*   Updated: 2024/01/09 18:33:01 by juli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <iso646.h>

int	main(void)
{
	int		fd[2]; // write into 1, read from 0
	char	buf[20];
	pid_t	id;

	id = fork();
	if (pipe(fd) == -1)
	{
		printf("Error with creating pipe\n");
		return (-1);
	}
	// if (id != 0)
	// 	wait(NULL);
	if (id == 0) // Child sends
	{
		int x = 1;
		printf("Child sent: (%d)\n", x);
		close(fd[0]);
		write(fd[1], &x, sizeof(int));
		close(fd[1]);
	}
	else // Parent recievs
	{
		int y;
		// wait(NULL);
		close(fd[1]);
		read(fd[0], &y, sizeof(int));
		close(fd[0]);
		printf("Parent received: (%d)\n", y);
	}
}