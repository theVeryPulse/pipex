/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FIFO_usage_write.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juli <juli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 17:25:31 by juli              #+#    #+#             */
/*   Updated: 2024/01/09 18:22:44 by juli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// How to use FIFO files to communicate between processes in C 
//    (https://www.youtube.com/watch?v=dniz3nEOEjU)
// First build a FIFO with $mkfifo sum

#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int	main(void)
{
	int	fd;
	const int	data[5] = {5, 4, 3, 2, 1};
	
	fd = open("sum", O_WRONLY);
	if (fd == -1)
	{
		if (errno == ENOENT)
			printf("FIFO not found\n");
		return (1);
	}
	if (write(fd, data, sizeof(int) * 5) == 1)
			return (2);
	for (int i = 0; i < 5; i++)
		printf("Wrote %d\n", data[i]);
	close(fd);

	return (0);
}