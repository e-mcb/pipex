/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 22:36:43 by mzutter           #+#    #+#             */
/*   Updated: 2025/02/11 22:37:30 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "libft/libft.h"

void ft_error(const char *message)
{
	perror(message);
	exit(EXIT_FAILURE);
}

char *ft_pathfinder(char *cmd, char **envp)
{
	char **possible_paths;
	char *final_path;
	char *tmp_path;
	int i;

	i = 0;
	while (envp[i] && ft_strnstr(envp[i], "PATH=", 5) == NULL)
		i++;
	if (!envp[i])
		return NULL;

	possible_paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (possible_paths[i])
	{
		tmp_path = ft_strjoin(possible_paths[i], "/");
		final_path = ft_strjoin(tmp_path, cmd);
		free(tmp_path);

		if (access(final_path, F_OK) == 0)
		{
			i = -1;
			while (possible_paths[++i])
				free(possible_paths[i]);
			free(possible_paths);
			return final_path;
		}

		free(final_path);
		i++;
	}

	i = -1;
	while (possible_paths[++i])
		free(possible_paths[i]);
	free(possible_paths);

	return NULL;
}

void ft_exec(char *argv, char **envp)
{
	char **cmd;
	char *path;
	int i;

	cmd = ft_split(argv, ' ');
	path = ft_pathfinder(cmd[0], envp);
	if (!path)
	{
		i = 0;
		while (cmd[i])
			free(cmd[i++]);
		free(cmd);
		ft_error("Command not found");
	}
	if (execve(path, cmd, envp) == -1)
		ft_error("execve failed");
}

void ft_parent_process(char **argv, char **envp, int *fd)
{
	int file_output;

	file_output = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (file_output == -1)
		ft_error("Failed to open output file");
	dup2(fd[0], STDIN_FILENO);
	dup2(file_output, STDOUT_FILENO);
	close(fd[0]);
	close(fd[1]);
	close(file_output);
	ft_exec(argv[3], envp);
}

void ft_child_process(char **argv, char **envp, int *fd)
{
	int file_input;

	file_input = open(argv[1], O_RDONLY);
	if (file_input == -1)
		ft_error("Failed to open input file");
	dup2(fd[1], STDOUT_FILENO);
	dup2(file_input, STDIN_FILENO);
	close(fd[0]);
	close(fd[1]);
	close(file_input);
	ft_exec(argv[2], envp);
}

int main(int argc, char **argv, char **envp)
{
	int fd[2];
	pid_t pid;

	if (argc != 5)
	{
		ft_putstr_fd("Argument error\n", 2);
		ft_putstr_fd("Usage: ./pipex <file1> <cmd1> <cmd2> <file2>\n", 1);
		return EXIT_FAILURE;
	}

	if (pipe(fd) == -1)
		ft_error("Failed to create pipe");

	pid = fork();
	if (pid == -1)
		ft_error("Failed to fork");

	if (pid == 0)
		ft_child_process(argv, envp, fd);

	waitpid(pid, NULL, 0);
	ft_parent_process(argv, envp, fd);

	return EXIT_SUCCESS;
}
