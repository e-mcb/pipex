/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 23:52:42 by mzutter           #+#    #+#             */
/*   Updated: 2025/02/26 02:02:22 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	secure_dup2(int old_fd, int new_fd)
{
	if (dup2(old_fd, new_fd) == -1)
	{
		perror("dup2 failed");
		exit(EXIT_FAILURE);
	}
}

// void	handle_heredoc(int argc, char **argv)
// {
// 	int	out;

// 	out = ft_open(argv[argc - 1], 0);
// 	ft_handle_heredoc(argc, argv[2]);
// 	secure_dup2(out, STDOUT_FILENO);
// }

// void	handle_input_output(int argc, char **argv)
// {
// 	int	in;
// 	int	out;

// 	in = ft_open(argv[1], 2);
// 	out = ft_open(argv[argc - 1], 1);
// 	secure_dup2(in, STDIN_FILENO);
// }

// prints an error message and exits the program
// only used if the exit status is supposed to be 1
void	ft_error(const char *message)
{
	perror(message);
	exit(EXIT_FAILURE);
}

char *trim_quotes(char *str) 
{
    int start;
    int end;
    char *trimmed;

    start = 0;
    end = ft_strlen(str) - 1;
    if (str[start] == '"' || str[start] == '\'')
        start++;
    if (str[end] == '"' || str[end] == '\'')
        end--;
    trimmed = malloc(end - start + 2);
    if (!trimmed)
        return (NULL);
    ft_strlcpy(trimmed, &str[start], end - start + 2);
    return (trimmed);
}
