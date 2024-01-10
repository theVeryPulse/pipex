/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FIFO_usage_read.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Philip <juli@student.42london.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 17:39:27 by Philip            #+#    #+#             */
/*   Updated: 2024/01/09 18:39:17 by Philip           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// How to use FIFO files to communicate between processes in C 
//    (https://www.youtube.com/watch?v=dniz3nEOEjU)

#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int	main(void)
{
	int	fd;
	int	data[5];

	fd = open("sum", O_RDONLY);
	if (fd == -1)
	{
		if (errno == ENOENT)
			printf("FIFO not found\n");
		return (1);
	}
	if (read(fd, data, sizeof(int) * 5) == -1)
			return (2);
	for (int i = 0; i < 5; i++)
		printf("Received %d\n", data[i]);
	close(fd);
	int sum = 0;
	for (int i = 0; i < 5; i++)
		sum += data[i];
	printf("Sum is %d\n", sum);
	return (0);
}