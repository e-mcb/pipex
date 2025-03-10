/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_and_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 21:33:48 by mzutter           #+#    #+#             */
/*   Updated: 2025/03/10 22:11:46 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// flushes the array containing all of the possible paths
static void	free_possible_paths(char **possible_paths)
{
	int	i;

	i = 0;
	while (possible_paths[i])
	{
		free(possible_paths[i]);
		i++;
	}
	free(possible_paths);
}

//iterates through envp to find the PATH= line 
//returns teh content of that line offset by 5 (after PATH=)
static char	*find_path_variable(char **envp)
{
	int	i;

	i = 0;
	while (envp[i] && ft_strnstr(envp[i], "PATH=", 5) == NULL)
		i++;
	if (!envp[i])
		return (NULL);
	return (envp[i] + 5);
}
// takes the path given as parameter which is 
// a possible directory where the command might be located. 
// Appends a / as well as the command to the path

static char	*construct_final_path(char *path, char *cmd)
{
	char	*tmp_path;
	char	*final_path;

	if (!cmd)
		return (NULL);
	tmp_path = ft_strjoin(path, "/");
	final_path = ft_strjoin(tmp_path, cmd);
	ft_putstr_fd(final_path, 0);
	free(tmp_path);
	return (final_path);
}

char	*ft_pathfinder(char *cmd, char **envp)
{
	char	**possible_paths;
	char	*final_path;
	char	*path_var;
	int		i;

	path_var = find_path_variable(envp);
	if (!path_var)
		return (NULL);
	possible_paths = ft_split(path_var, ':');
	i = 0;
	while (possible_paths[i])
	{
		final_path = construct_final_path(possible_paths[i], cmd);
		if (final_path != NULL && access (final_path, F_OK) == 0)
		{
			free_possible_paths(possible_paths);
			return (final_path);
		}
		free(final_path);
		i++;
	}
	free_possible_paths(possible_paths);
	return (NULL);
}

int	ft_exec_cmd(char *argv, char **envp)
{
	char	**cmd;
	char	*path;
	char	*trim;
	int		i;

	i = 0;
	cmd = ft_split2(argv, ' ');
	if (!cmd)
		return (ft_putstr_fd("Error:Malloc failed\n", 2), 0);
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
	path = helper_path(cmd, envp);
	if (execve(path, cmd, envp) == -1)
		return (ft_error("execve failed\n"), 1);
	return (0);
}
