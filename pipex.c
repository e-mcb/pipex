/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 22:36:43 by mzutter           #+#    #+#             */
/*   Updated: 2025/02/25 01:12:25 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// prints an error message and exits the program
// only used if the exit status is supposed to be 1

void	ft_error(const char *message)
{
	perror(message);
	exit(EXIT_FAILURE);
}
// splits the command from its options
// finds the path of the command
// executes it

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to remove the quotes from a string
static char *remove_quotes(char *str)
{
    int start = 0;
    int end = strlen(str) - 1;

    // Check if the first and last characters are quotes (' or ")
    if ((str[start] == '"' || str[start] == '\'') && (str[end] == '"' || str[end] == '\''))
    {
        start++;  // Skip the first quote
        end--;    // Skip the last quote
    }

    // Create a new string without quotes
    char *trimmed = malloc(end - start + 2);  // +1 for null terminator
    if (!trimmed)
        return NULL;
    
    // Copy the content between the quotes
    strncpy(trimmed, &str[start], end - start + 1);
    trimmed[end - start + 1] = '\0';  // Null-terminate the string

    return trimmed;
}

void ft_exec_cmd(char *argv, char **envp)
{
    char **cmd;
    char *path;
    int i;

    // Split the command into an array of strings
    cmd = ft_split2(argv, ' ');

    // Iterate over each string in cmd and trim quotes
    i = 0;
    while (cmd[i]) 
    {
        char *trimmed = remove_quotes(cmd[i]);
        if (trimmed)
        {
            free(cmd[i]);    // Free the old string with quotes
            cmd[i] = trimmed;  // Update cmd[i] with the trimmed string
        }
        i++;  // Move to the next string
    }

    // Find the path for the command
    path = ft_pathfinder(cmd[0], envp);
    if (!path)
    {
        i = 0;
        while (cmd[i])
            free(cmd[i++]);
        free(cmd);
        perror("command not found");
        exit(127);
    }

    // Execute the command
    if (execve(path, cmd, envp) == -1)
        ft_error("execve failed");
}

// void	ft_exec_cmd(char *argv, char **envp)
// {
// 	char	**cmd;
// 	char	*path;
// 	int		i;

// 	cmd = ft_split2(argv, ' ');
// 	path = ft_pathfinder(cmd[0], envp);
// 	if (!path)
// 	{
// 		i = 0;
// 		while (cmd[i])
// 			free(cmd[i++]);
// 		free(cmd);
// 		perror("command not found");
// 		exit(127);
// 	}
// 	if (execve(path, cmd, envp) == -1)
// 		ft_error("execve failed");
// }

// opens the file2, creates it if it doesnt exist
// deletes its content if it exists
// redirect STDIN to fd[0] (read) 
// to read from the child process
// redirects STDOUT to file2 (write)
// closes file2, fd[0] and fd[1]
// executes the cmd2
// writes its output into file2

void	ft_parent_process(char **argv, char **envp, int *fd)
{
	int	file_output;

	file_output = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (file_output == -1)
		ft_error("Failed to open output file");
	dup2(fd[0], STDIN_FILENO);
	dup2(file_output, STDOUT_FILENO);
	close(fd[0]);
	close(fd[1]);
	close(file_output);
	ft_exec_cmd(argv[3], envp);
}

// opens file1 in read only mode
// redirects STDOUT to fd[1]
// redirects STDIN to file1
// closes file1, fd[0] and fd[1]
// executes the cmd1 which
// will be sent back to the parent process

void	ft_child_process(char **argv, char **envp, int *fd)
{
	int	file_input;

	file_input = open(argv[1], O_RDONLY);
	if (file_input == -1)
		ft_error("Failed to open input file");
	dup2(fd[1], STDOUT_FILENO);
	dup2(file_input, STDIN_FILENO);
	close(fd[0]);
	close(fd[1]);
	close(file_input);
	ft_exec_cmd(argv[2], envp);
}

int	main(int argc, char **argv, char **envp)
{
	int		fd[2];
	pid_t	pid;

	if (argc != 5)
		ft_error("Invalid format");
	if (pipe(fd) == -1)
		ft_error("Failed to create pipe");
	pid = fork();
	if (pid == -1)
		ft_error("Failed to fork");
	if (pid == 0)
		ft_child_process(argv, envp, fd);
	waitpid(pid, NULL, 0);
	ft_parent_process(argv, envp, fd);
	return (EXIT_SUCCESS);
}