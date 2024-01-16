/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_funcs_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Philip <juli@student.42london.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 22:54:23 by Philip            #+#    #+#             */
/*   Updated: 2024/01/16 22:44:59 by Philip           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	exit_status_of_last_cmd(pid_t id_last)
{
	int	stat_val;

	waitpid(id_last, &stat_val, 0);
	if (WIFEXITED(stat_val))
		stat_val = WEXITSTATUS(stat_val);
	return (stat_val);
}

void	read_from_stdin(t_cmd *cmds, int *pipes)
{
	char	*line;

	while (1)
	{
		line = get_next_line(STDIN_FILENO);
		if (is_delimiter(line, cmds[0].argv[0]))
			break ;
		if (line)
		{
			write(pipes[1], line, ft_strlen(line));
		}
		free_setnull(1, (void **)&line);
	}
	free_setnull(1, (void **)&line);
	close_all_pipes(pipes);
	exit (0);
}
