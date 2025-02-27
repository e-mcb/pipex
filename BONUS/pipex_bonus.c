/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 21:28:05 by mzutter           #+#    #+#             */
/*   Updated: 2025/02/26 02:49:40 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	ft_open(char *argv, int i)
{
	int	file;

	file = 0;
	if (i == 0)
		file = open(argv, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (i == 1)
		file = open(argv, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (i == 2)
		file = open(argv, O_RDONLY, 0644);
	if (file == -1)
		ft_error("failed to open the file");
	return (file);
}

void	ft_child_process(char *argv, char **envp)
{
	pid_t	pid;
	int		fd[2];

	if (pipe(fd) == -1)
		ft_error("Failed to create pipe");
	pid = fork();
	if (pid == -1)
		ft_error("Failed to fork");
	if (pid == 0)
	{
		close (fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		ft_exec_cmd(argv, envp);
	}
	else
	{
		close (fd[1]);
		dup2(fd[0], STDIN_FILENO);
		waitpid(pid, NULL, 0);
	}
}

void	ft_gnl_heredoc(int fd[2], char *limiter)
{
	char	*line;

	close(fd[0]);
	while (1)
	{
		line = get_next_line(0);
		if (!line || (limiter[0] == 0 && line[0] == '\n')
			|| ft_strncmp(line, limiter, ft_strlen(limiter)) == 0)
		{
			free(line);
			exit(EXIT_SUCCESS);
		}
		ft_putstr_fd(line, fd[1]);
		free(line);
	}
}

void	ft_handle_heredoc(int argc, char *limiter)
{
	pid_t	pid;
	int		fd[2];

	if (argc < 6)
		ft_error("Format error: Expected \"./pipex"
			"here_doc LIMITER cmd cmd1 file\"");
	if (pipe(fd) == -1)
		ft_error("Failed to create pipe");
	pid = fork ();
	if (pid == 0)
		ft_gnl_heredoc(fd, limiter);
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		wait(NULL);
	}
}

int	main(int argc, char **argv, char **envp)
{
	int	i;
	int	out;

	if (argc < 5)
		ft_error("Invalid format");
	else
	{
		if (ft_strncmp(argv[1], "here_doc", 9) == 0)
		{
			i = 3;
			open_heredoc(argc, argv);
		}
		else
		{
			i = 2;
			out = handle_input_output(argc, argv);
		}
		while (i < argc - 2)
			ft_child_process(argv[i++], envp);
		secure_dup2(out, STDOUT_FILENO);
		ft_exec_cmd(argv[argc - 2], envp);
	}
	exit(EXIT_SUCCESS);
}
