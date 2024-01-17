/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_funcs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Philip <juli@student.42london.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 22:16:10 by Philip            #+#    #+#             */
/*   Updated: 2024/01/17 16:26:59 by Philip           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * @return cmds(t_cmd[]) NULL terminated. Exit with 1 when malloc fails.
 */
t_cmd	*format_cmds(int argc, char **argv, char **envp)
{
	int		i;
	t_cmd	*cmds;
	int		cmd_count;
	char	**cmd_start;

	cmd_count = argc - 3;
	cmd_start = argv + 2;
	cmds = (t_cmd *)ft_calloc(cmd_count + 1, sizeof(t_cmd));
	if (!cmds)
		exit (1);
	i = 0;
	while (i < cmd_count)
	{
		cmds[i].argv = ft_split(cmd_start[i], ' ');
		cmds[i].path = format_string("/usr/bin/%s", cmds[i].argv[0]);
		cmds[i].envp = envp;
		cmds[i].is_delimiter = false;
		i++;
	}
	if (ft_strncmp(argv[1], "here_doc", ft_strlen("here_doc") + 1) == 0)
		cmds[0].is_delimiter = true;
	return (cmds);
}

/**
 * @brief Checks if all commands exist in /usr/bin, free allocated memory and 
 * exit the program if a command cannot be found.
 */
void	check_cmds(t_cmd *cmds, int cmd_count)
{
	int	i;

	i = 0;
	while (i < cmd_count)
	{
		if (!(cmds[i].is_delimiter) && access(cmds[i].path, F_OK | X_OK) == -1)
		{
			ft_putstr_fd(cmds[i].argv[0], STDERR_FILENO);
			ft_putendl_fd(": command not found", STDERR_FILENO);
			free_cmds(cmds);
			exit (127);
		}
		i++;
	}
}

void	free_cmds(t_cmd *cmds)
{
	int	i;
	int	j;

	i = 0;
	while (cmds[i].argv)
	{
		j = 0;
		while (cmds[i].argv[j])
			free(cmds[i].argv[j++]);
		free(cmds[i].argv);
		free(cmds[i].path);
		i++;
	}
	free(cmds);
}
