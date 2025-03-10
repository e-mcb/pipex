/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 23:52:42 by mzutter           #+#    #+#             */
/*   Updated: 2025/03/10 21:59:17 by mzutter          ###   ########.fr       */
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

char	*trim_quotes(char *str)
{
	int		start;
	int		end;
	char	*trimmed;

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

static void	free_paths(char **cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		free(cmd[i]);
		i++;
	}
	free(cmd);
	ft_putstr_fd("command not found\n", 2);
	exit(127);
}

char	*helper_path(char **cmd, char **envp)
{
	char	*path;

	if (cmd[0][0] == '/')
		path = cmd[0];
	else
		path = ft_pathfinder(cmd[0], envp);
	if (!path)
		return (free_paths(cmd), NULL);
	return (path);
}

void	secure_dup2(int old_fd, int new_fd)
{
	if (dup2(old_fd, new_fd) == -1)
	{
		perror("dup2 failed");
		exit(EXIT_FAILURE);
	}
}
