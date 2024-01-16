/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Philip <juli@student.42london.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 15:35:38 by Philip            #+#    #+#             */
/*   Updated: 2024/01/16 17:15:57 by Philip           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft.h"
# include <errno.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdarg.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>

typedef struct s_cmd
{
	char	**argv;
	char	*path;
	char	**envp;
	bool	is_delimiter;
}	t_cmd;

// limiter set as the first cmd in the array with is_limiter as true;
// In exec_first_command function, checks if first command is limiter and use 
// correspoding codes.

typedef enum e_mode
{
	GET,
	FREE
}	t_mode;


#endif