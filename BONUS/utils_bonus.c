/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 23:34:35 by mzutter           #+#    #+#             */
/*   Updated: 2025/02/25 23:12:24 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	ft_error(const char *message)
{
	perror(message);
	exit(EXIT_FAILURE);
}

static char	*trim_quotes(char *str)
{
	int		start;
	int		end;
	char	*trimmed;

	start = 0;
	end = (ft_strlen (str) - 1);
	if (str[start] == '"' || str[start] == '\'')
		start++;
	if (str[end] == '"' || str[end] == '\'')
		end--;
	trimmed = malloc(end - start + 2);
	if (!trimmed)
		return (NULL);
	ft_strlcpy(trimmed, &str[start], end - start + 1);
	trimmed[end - start + 1] = '\0';
	return (trimmed);
}

static void	free_paths(char **cmd)
{
	int	i;

	i = 0;
	while ((*cmd)[i])
	{
		free((*cmd)[i]);
		i++;
	}
	free(*cmd);
	perror("command not found");
	exit(127);
}

void	ft_exec_cmd(char *argv, char **envp)
{
	char	**cmd;
	char	*path;
	char	*trim;
	int		i;

	i = 0;
	cmd = ft_split2(argv, ' ');
	while (cmd[i])
	{
		trim = trim_quotes(cmd[i]);
		if (trim)
		{
			free(cmd[i]);
			cmd[i] = trim;
		}
		i++;
	}
	path = ft_pathfinder(cmd[0], envp);
	if (!path)
	{
		free_paths(&cmd);
	}
	if (execve(path, cmd, envp) == -1)
		ft_error("execve failed");
}
