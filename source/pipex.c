/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Philip <juli@student.42london.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 15:28:54 by Philip            #+#    #+#             */
/*   Updated: 2024/01/10 22:39:12 by Philip           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// $ ./pipex infile "ls -l" "wc -l" outfile 
// follows 
// $ < file1 ls -l | wc -l > file2

// $ ./pipex file1 cmd1 cmd2 cmd3 ... cmdn file2
// follows
// $ < file1 cmd1 | cmd2 | cmd3 ... | cmdn > file2

// ./pipex here_doc END cmd cmd1 file
// follows
// $ cmd <<END | cmd1 >> file

// PATH=/nfs/homes/juli/bin:/nfs/homes/juli/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin

void	err_msg(const char *s)
{
	write(STDERR_FILENO, s, ft_strlen(s));
}

int	main(int argc, char **argv)
{
	int		fildes[2];
	pid_t	id;
	int		infile_fd;

	if (argc < 5)
	{
		err_msg("command incomplete\n");
		return (1);
	}

	// infile_fd = open()

	// if (pipe(fildes) == -1)
	// 	return (1);

	// id = fork();
	// if (id == -1)
	// 	return (2);

	// /* See if infile exists */
	// /* See if both target programs exist */

	// /* Run the programs with the flags */
	// /*  */
	// if (id == 0)
	// {
	// 	execve("");
	// }
	// else
	// {

	// }

}