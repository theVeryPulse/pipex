/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Philip <juli@student.42london.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 22:21:33 by Philip            #+#    #+#             */
/*   Updated: 2024/01/17 15:01:01 by Philip           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_setnull(int count, void **ptr, ...)
{
	int		i;
	va_list	ap;
	void	**this;

	va_start(ap, ptr);
	if (ptr || *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
	i = 1;
	while (i < count)
	{
		this = va_arg(ap, void **);
		if (!this || *this)
			continue ;
		free(*this);
		*this = NULL;
	}
}

bool	is_delimiter(char *line, char *delimiter)
{
	size_t	i;

	if (!line || !delimiter)
		return (false);
	i = 0;
	while (delimiter[i])
	{
		if (line[i] != delimiter[i])
		{
			return (false);
		}
		i++;
	}
	return (line[i] == '\n' && line[i + 1] == '\0');
}

bool	not_enough_args(int argc, char **argv)
{
	if (argc < 5)
	{
		return (true);
	}
	if (ft_strncmp(argv[1], "here_doc", ft_strlen("here_doc") + 1) == 0
		&& argc < 6)
	{
		return (true);
	}
	return (false);
}
