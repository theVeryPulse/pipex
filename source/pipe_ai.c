/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_ai.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Philip <juli@student.42london.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 22:04:10 by Philip            #+#    #+#             */
/*   Updated: 2024/01/10 22:04:21 by Philip           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Reference only

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	if (argc < 5)
	{
		printf("Usage: %s infile command1 command2 outfile\n", argv[0]);
		return 1;
	}

	// Open the input and output files
	FILE *infile = fopen(argv[1], "r");
	FILE *outfile = fopen(argv[4], "w");

	if (infile == NULL || outfile == NULL)
	{
		perror("Error opening files");
		return 1;
	}

	// Create a pipe
	int pipefd[2];
	if (pipe(pipefd) == -1)
	{
		perror("Error creating pipe");
		return 1;
	}

	// Fork the first child process to execute command1
	pid_t pid1 = fork();
	if (pid1 == -1)
	{
		perror("Error forking process");
		return 1;
	}
	else if (pid1 == 0) 
	{
		// Child process 1
		// Redirect stdout to the write end of the pipe
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);

		// Execute command1
		execlp(argv[2], argv[2], NULL);
		perror("Error executing command1");
		return 1;
	}

	// Fork the second child process to execute command2
	pid_t pid2 = fork();
	if (pid2 == -1)
	{
		perror("Error forking process");
		return 1;
	}
	else if (pid2 == 0)
	{
		// Child process 2
		// Redirect stdin to the read end of the pipe
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);

		// Redirect stdout to the output file
		dup2(fileno(outfile), STDOUT_FILENO);
		fclose(outfile);

		// Execute command2
		execlp(argv[3], argv[3], NULL);
		perror("Error executing command2");
		return 1;
	}

	// Parent process
	close(pipefd[0]);
	close(pipefd[1]);

	// Wait for both child processes to finish
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);

	// Close the input and output files
	fclose(infile);
	fclose(outfile);

	return 0;
}
