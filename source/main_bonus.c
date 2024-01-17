/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Philip <juli@student.42london.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 22:27:28 by Philip            #+#    #+#             */
/*   Updated: 2024/01/17 15:15:35 by Philip           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	if (not_enough_args(argc, argv))
	{
		ft_putendl_fd("incorrect number of commands", STDERR_FILENO);
		return (1);
	}
	exec_commands(argc, argv, envp);
	return (0);
}
