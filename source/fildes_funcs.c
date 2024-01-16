/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fildes_funcs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Philip <juli@student.42london.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 22:23:31 by Philip            #+#    #+#             */
/*   Updated: 2024/01/16 22:24:31 by Philip           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * @return File descriptor of input file, or 0 (STDIN_FILENO) when infile name
 * is "here_doc".
 * Returns -1 when open fails
*/
int	get_input_fd(const char *input_name)
{
	int	infile_fd;

	if (ft_strncmp(input_name, "here_doc", ft_strlen("here_doc") + 1) == 0)
	{
		infile_fd = STDIN_FILENO;
		ft_printf("here doc mode\n");
	}
	else
	{
		infile_fd = open(input_name, O_RDONLY);
	}
	if (infile_fd == -1)
	{
		perror(input_name);
		exit (1);
	}
	return (infile_fd);
}

int	get_outfile_fd(t_cmd *cmds, char *outfile)
{
	int	outfile_fd;

	if (cmds[0].is_delimiter)
		outfile_fd = open(outfile, O_CREAT | O_WRONLY | O_APPEND, 0777);
	else
		outfile_fd = open(outfile, O_CREAT | O_WRONLY | O_TRUNC, 0777);
	return (outfile_fd);
}
