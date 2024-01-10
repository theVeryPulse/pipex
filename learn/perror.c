/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perror.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Philip <juli@student.42london.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 22:05:56 by Philip            #+#    #+#             */
/*   Updated: 2024/01/10 22:09:34 by Philip           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int	main(void)
{
	int	fd;

	fd = open("null", O_RDONLY);
	if (fd == -1)
	{
		perror("Error");
		return (1);
	}
	close(fd);
	return (0);
}