/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juli <juli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 17:15:38 by Philip            #+#    #+#             */
/*   Updated: 2024/01/08 21:31:32 by juli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int	main(void)
{
	int	id;

	id = fork();
	if (id < 0)
	{
		perror("fork fail");
		exit(1);
	}
	wait(NULL); // Waits for a child process to complete

	if (id == 0)
		printf("I am the child, the parent is waiting\n");
	else if (id != 0)
		printf("I am the parent, the child should be finished\n");

	if (wait(NULL) == -1 && errno == ECHILD)
		printf("The parent does not have any children now\n");

}
