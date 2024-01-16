/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_funcs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Philip <juli@student.42london.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 22:21:10 by Philip            #+#    #+#             */
/*   Updated: 2024/01/16 22:45:13 by Philip           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * @param pipe_count (int) In this case (cmd_count - 1) or (argc - 4)
 * @return int pipes[pipe_count * 2]. Array is 0 terminated.
 */
int	*build_pipes(int pipe_count)
{
	int	i;
	int	*pipes;

	pipes = (int *)ft_calloc(pipe_count + 1, sizeof(int) * 2);
	i = 0;
	while (i < pipe_count)
	{
		if (pipe(&pipes[i * 2]) == -1)
		{
			perror("pipex");
			return (NULL);
		}
		i++;
	}
	return (pipes);
}

/**
 * @note Function ends upon value zero, so STDIN_FILENO should not be inside the
 * array.
 */
void	close_all_pipes(int	*pipes)
{
	int	i;

	if (!pipes)
		return ;
	i = 0;
	while (pipes[i])
		close(pipes[i++]);
}
