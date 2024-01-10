/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   04_two-way-comm-pipe.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Philip <juli@student.42london.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 18:41:02 by Philip            #+#    #+#             */
/*   Updated: 2024/01/09 19:09:26 by Philip           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <wait.h>

#define P_READ (0)
#define P_WRITE (1)
int	main(void)
{
	int pipe_p2c[2];
	int pipe_c2p[2];
	pid_t	id;

	if (pipe(pipe_p2c) == -1) // Build the pipe from parent to child
		return (1);
	if (pipe(pipe_c2p) == -1) // Build the pipe from child to parent
		return (1);

	id = fork(); // Build the child process
	if (id == -1)
		return (2);

	if (id == 0) // Child process
	{
		close(pipe_c2p[P_READ]);
		close(pipe_p2c[P_WRITE]);
		int	x;
		if (read(pipe_p2c[P_READ], &x, sizeof(int)) == -1)
			return (3);
		printf("Child receives: (%d)\n", x);
		x *= 4;
		if (write(pipe_c2p[P_WRITE], &x, sizeof(int)) == -1)
			return (4);
		printf("Child sends: (%d)\n", x);
	}
	else // Parent process
	{
		close(pipe_c2p[P_WRITE]);
		close(pipe_p2c[P_READ]);
		int	y = 5;
		if (write(pipe_p2c[P_WRITE], &y, sizeof(int)) == -1)
			return (5);
		printf("Parent sends: (%d)\n", y);
		wait(NULL);
		if (read(pipe_c2p[P_READ], &y, sizeof(int)) == -1)
			return (6);
		printf("Parent receives: (%d)\n", y);
	}
	close(pipe_p2c[0]);
	close(pipe_p2c[1]);
	return (0);
}
