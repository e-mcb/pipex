/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 22:36:43 by mzutter           #+#    #+#             */
/*   Updated: 2025/03/10 22:39:59 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	first_process(char **argv, char **envp, int *end)
{
	int	f1;
	int	result;

	if (!argv[2] || argv[2][0] == '\0')
	{
		ft_putstr_fd("Invalid argument.\n", 2);
		exit(EXIT_FAILURE);
	}
	f1 = open(argv[1], O_RDONLY);
	if (f1 == -1)
		ft_error("Error: failed to open source file.\n");
	if (dup2(f1, STDIN_FILENO) < 0)
		ft_error("Error: Dup2 failed\n");
	if (dup2(end[1], STDOUT_FILENO) < 0)
		ft_error("Error: Dup2 failed\n");
	close(f1);
	close(end[1]);
	result = ft_exec_cmd(argv[2], envp);
	if (result == 1)
		exit(EXIT_FAILURE);
	exit (1);
}

static void	second_process(char **argv, char **envp, int *end)
{
	int	f2;
	int	result;

	if (!argv[3] || argv[3][0] == '\0')
	{
		ft_putstr_fd("Invalid argument.\n", 2);
		exit(EXIT_FAILURE);
	}
	f2 = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (f2 == -1)
		ft_error("Failed to open output file.\n");
	if (dup2(end[0], STDIN_FILENO) < 0)
		ft_error("Dup2 failed\n");
	if (dup2(f2, STDOUT_FILENO) < 0)
		ft_error("Dup2 failed\n");
	close(f2);
	close(end[0]);
	result = ft_exec_cmd(argv[3], envp);
	if (result == 1)
		exit(EXIT_FAILURE);
	exit (1);
}

int	main(int argc, char **argv, char **envp)
{
	int		end[2];
	pid_t	pid1[2];

	if (argc != 5)
		return (ft_putstr_fd("Usage: ./pipex file cmd cmd file\n", 2), 0);
	if (pipe(end) == -1)
		ft_error("Pipe error\n");
	pid1[0] = fork();
	if (pid1[0] < 0)
		ft_error("Fork error\n");
	if (pid1[0] == 0)
	{
		close(end[0]);
		first_process(argv, envp, end);
	}
	pid1[1] = fork();
	if (pid1[1] < 0)
		ft_error("Fork error\n");
	waitpid(pid1[0], NULL, 0);
	if (pid1[1] == 0)
	{
		close(end[1]);
		second_process(argv, envp, end);
	}
	return (close(end[0]), close(end[1]), waitpid(pid1[1], NULL, 0), 0);
}
