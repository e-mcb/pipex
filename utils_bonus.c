/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 23:34:35 by mzutter           #+#    #+#             */
/*   Updated: 2025/02/20 21:55:30 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	ft_error(const char *message)
{
	perror(message);
	exit(EXIT_FAILURE);
}

void	ft_exec_cmd(char *argv, char **envp)
{
	char	**cmd;
	char	*path;
	int		i;

	cmd = ft_split(argv, ' ');
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
	if (execve(path, cmd, envp) == -1)
		ft_error("execve failed");
}

char **ft_special_split(char const *str, char *sep)
{
	
}

static size_t ft_count_words(char const *str, char const *sep)
{
	size_t	i;
	size_t	j;
	size_t	word_count;

	if (!str[0])
		return (0);
	i = 0;
	word_count = 0;
	while (s[i++] != 0)
	{
		while (sep[j] != 0)
		{
			if (sep[j] == s[i])
				word_count++;
		}
	}
}

#include <stdio.h>
#include <string.h>

int main() {
    char str[] = "hello \"world\" and 'goodbye'";    // Original string with quotes
    int len = ft_strlen(str);
    
    // Create a new string with enough space for the escaped characters (extra space for backslashes)
    char new_str[2 * len + 1];   // In the worst case, each character might get escaped, so we double the size

    int i = 0, j = 0;  // i is the index for original string, j is the index for new string

    // Loop through the original string
    while (i < len) {
        // Check if the character is a quote (either single or double)
        if (str[i] == '\'' || str[i] == '\"') {
            new_str[j++] = '\\';  // Add the escape character
        }
        
        // Copy the current character to the new string
        new_str[j++] = str[i];
        i++;  // Increment the index for the original string
    }

    // Null-terminate the new string
    new_str[j] = '\0';

    // Print the original and modified strings
    printf("Original string: %s\n", str);
    printf("Modified string: %s\n", new_str);

    return 0;
}
