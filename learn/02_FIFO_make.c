/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FIFO_make.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Philip <juli@student.42london.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 21:29:07 by Philip            #+#    #+#             */
/*   Updated: 2024/01/09 18:36:20 by Philip           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

int	main(void)
{
	if (mkfifo("myfifo1", 0777) == -1) // Octal 777, read write exec
	{
		if (errno != EEXIST)
		{
			printf("Could not create fifo file\n");
			return (1);
		}
	}

	printf("Opening FIFO...\n");
	int fd = open("myfifo1", O_WRONLY);
	/* Note: opening read or write side of the FIFO blocks until the other side
	   is also opened. */
	printf("Opened\n");

	char *data = "4242\n";
	printf("Writing to FIFO...\n");
	if (write(fd, data, strlen(data)) == -1)
	{
		printf("Write Error\n");
		return (2);
	}
	else
		printf("Writing complete\n");

	printf("Closing FIFO...\n");
	close(fd);
	printf("Closed\n");
}