/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   07_01_send_string_thru_pipe.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Philip <juli@student.42london.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 14:33:18 by Philip            #+#    #+#             */
/*   Updated: 2024/01/10 14:42:53 by Philip           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <string.h>

int	main(void)
{
	int	fildes[2];
	int	id;

	if (pipe(fildes) == -1)
		return (1);
	
	id = fork();
	if (id == -1)
		return (2);
	
	if (id == 0) 
	{
		char	data[100];

		close(fildes[0]);
		fgets(data, 100, stdin);
		write(fildes[1], data, strlen(data) + 1);
		printf("Child sends string: %s\n", data);
		close(fildes[1]);
	}
	else
	{
		char	data[100];

		close(fildes[1]);
		read(fildes[0], data, 100);
		printf("Parent receives string: %s\n", data);
		close(fildes[0]);
	}
}