/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   05_exec-another-program.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Philip <juli@student.42london.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 19:20:41 by Philip            #+#    #+#             */
/*   Updated: 2024/01/09 21:47:12 by Philip           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// "How to execute another program in C (using exec)"
// https://www.youtube.com/watch?v=OVFEWSP7n8c

// "System Calls in linux | exec | execv"
// https://www.youtube.com/watch?v=5BE9qM79fgM

/* Using execve()
	The following example passes  arguments  to  the  ls command in the cmd
	array, and specifies the environment for the new process image using the
	env argument.

		   #include <unistd.h>

		   int ret;
		   char *cmd[] = { "ls", "-l", (char *)0 };
		   char *env[] = { "HOME=/usr/home", "LOGNAME=home", (char *)0 };
		   ...
		   ret = execve ("/bin/ls", cmd, env); */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int	main(void)
{
	// int ret;
	// char *cmd[] = { "ls", "-l", (char *)0 };
	// char *env[] = { "HOME=/usr/home", "LOGNAME=home", (char *)0 };
	// ret = execve("/bin/ls", cmd, env);
	char *args[] = {"hello", "Hello", "world\n", NULL};

	printf("Running %s\n", __FILE__);
	execve("hello", args, NULL); // Runs a executable "hello" under same dir
	printf("Error");
}