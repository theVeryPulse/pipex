/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   07_send_array_thru_pipe.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Philip <juli@student.42london.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 13:00:34 by Philip            #+#    #+#             */
/*   Updated: 2024/01/10 13:17:50 by Philip           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <wait.h>

int	main(void)
{
	pid_t	id;
	int		my_pipe[2];

	if (pipe(my_pipe) == -1)
		return (1);

	// Important: all preparations that child should have should be done before
	// fork.
	id = fork();
	if (id == -1)
		return (1);
	
	if (id == 0)
	{
		close(my_pipe[0]);
		int	c_ary[] = {5, 4, 3, 2, 1};

		printf("Child sends: ");
		for (int i = 0; i < 5; i++)
			printf("%d ", c_ary[i]);
		printf("\n");

		write(my_pipe[1], c_ary, sizeof(int) * 5);
		close(my_pipe[1]);
	}
	else
	{
		close(my_pipe[1]);
		int	p_ary[5];
		wait(NULL);
		read(my_pipe[0], p_ary, sizeof(int) * 5);

		printf("Parent receives: ");
		for (int i = 0; i < 5; i++)
			printf("%d ", p_ary[i]);
		printf("\n");

		close(my_pipe[0]);
	}

}