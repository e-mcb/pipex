/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 21:28:05 by mzutter           #+#    #+#             */
/*   Updated: 2025/02/20 22:15:38 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

// opens the files with the correct flags
// i == 0 for heredoc (append)
// i == 1 for output file
//i == 2 for input file
int	ft_open(char *argv, int i)
{
	int file;

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

// handles the communication inbetween pipes
// if in the child process, the STDOUT will move to the write
// end of the pipe
// then when in the parent process, then the STDIN will
// be moved to the read end of the pipe
// for the subsequent command to handle
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

//writes the line received from STDIN
// until limiter is encountered

void ft_gnl_heredoc(int fd[2], char *limiter)
{
    char *line;

    close(fd[0]);
    while (1)
    {
        line = get_next_line(0);
        if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0)
        {
            free(line);
            exit(EXIT_SUCCESS);
        }
        ft_putstr_fd(line, fd[1]);
        free(line);
    }
}


void ft_handle_heredoc(int argc, char *limiter)
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
// sed
// nested "" / ()
// processes each child process individually in
// a while loop as long as there are commands to execute
// immediately changes the STDIN to the input file
// and ends by changin the STDOUT to the output file 
// before executing the last command
int main (int argc, char **argv, char **envp)
{
	int	i;
	int	in;
	int	out;

	if (argc > 4)
	{
		if (ft_strncmp(argv[1], "here_doc", 9) == 0)
		{
			i = 3;
			out = ft_open(argv[argc - 1], 0);
			ft_handle_heredoc(argc, argv[2]);
		}
		else
		{
			i = 2;
			out = ft_open(argv[argc - 1], 1);
			in = ft_open(argv[1], 2);
			dup2(in, STDIN_FILENO);
		}
		while ( i < argc - 2)
			ft_child_process(argv[i++], envp);
		dup2(out, STDOUT_FILENO);
		ft_exec_cmd(argv[argc - 2], envp);
	}
	exit(EXIT_SUCCESS);
}