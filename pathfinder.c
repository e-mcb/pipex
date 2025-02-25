/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathfinder.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 20:20:05 by mzutter           #+#    #+#             */
/*   Updated: 2025/02/25 00:56:45 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// flushes the array containing all of the possible paths

void	free_possible_paths(char **possible_paths)
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

char	*find_path_variable(char **envp)
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

char	*construct_final_path(char *path, char *cmd)
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
// splits all the directories of 
// the PATH= line of envp to append the command to them
// tries to access all the paths to 
// check for their existence before reutrning the correct one

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

static char **ft_malloc_error(char **tab)
{
    size_t i;

    i = 0;
    while (tab[i])
    {
        free(tab[i]);
        i++;
    }
    free(tab);
    return (NULL);
}

static bool is_quote(char c)
{
    return c == '"' || c == '\'' || c == '{';
}

static bool is_closing_quote(char c, char opening_quote)
{
    if (opening_quote == '"' && c == '"')
        return true;
    if (opening_quote == '\'' && c == '\'')
        return true;
    if (opening_quote == '{' && c == '}')
        return true;
    return false;
}

static size_t ft_nb_words(char const *s, char c)
{
    size_t i;
    size_t nb_words;
    bool in_quotes;
    char opening_quote;

    if (!s[0])
        return (0);
    i = 0;
    nb_words = 0;
    in_quotes = false;
    opening_quote = '\0';
    while (s[i] && s[i] == c)
        i++;
    while (s[i])
    {
        if (is_quote(s[i]) && !in_quotes)
        {
            in_quotes = true;
            opening_quote = s[i];
        }
        else if (is_closing_quote(s[i], opening_quote) && in_quotes)
        {
            in_quotes = false;
            opening_quote = '\0';
        }
        if (s[i] == c && !in_quotes)
        {
            nb_words++;
            while (s[i] && s[i] == c)
                i++;
            continue;
        }
        i++;
    }
    if (s[i - 1] != c || in_quotes)
        nb_words++;
    return (nb_words);
}

static void ft_get_next_word(char **next_word, size_t *next_word_len, char c)
{
    size_t i;
    bool in_quotes;
    char opening_quote;

    *next_word += *next_word_len;
    *next_word_len = 0;
    i = 0;
    in_quotes = false;
    opening_quote = '\0';
    while (**next_word && **next_word == c && !in_quotes)
        (*next_word)++;
    while ((*next_word)[i])
    {
        if (is_quote((*next_word)[i]) && !in_quotes)
        {
            in_quotes = true;
            opening_quote = (*next_word)[i];
        }
        else if (is_closing_quote((*next_word)[i], opening_quote) && in_quotes)
        {
            in_quotes = false;
            opening_quote = '\0';
        }
        if ((*next_word)[i] == c && !in_quotes)
            return;
        (*next_word_len)++;
        i++;
    }
}
char	*ft_strtrim(char const *s1, char const *set)
{
	int		start;
	int		end;
	char	*str;

	if (!s1 || !set)
		return (NULL);
	start = 0;
	end = ft_strlen(s1) - 1;
	while (ft_strchr(set, s1[start]) && start <= end)
		start++;
	if (start > end)
		return (ft_strdup(s1 + end + 1));
	while (ft_strchr(set, s1[end]) && end >= 0)
		end--;
	str = malloc(end - start + 2);
	if (!str)
		return (NULL);
	ft_strlcpy(str, &s1[start], end - start + 2);
	return (str);
}

char **ft_split2(char const *s, char c)
{
    char **tab;
    char *next_word;
    size_t next_word_len;
    size_t i;

    if (!s)
        return (NULL);
    tab = (char **)malloc(sizeof(char *) * (ft_nb_words(s, c) + 1));
    if (!tab)
        return (NULL);
    i = 0;
    next_word = (char *)s;
    next_word_len = 0;
    while (i < ft_nb_words(s, c))
    {
        ft_get_next_word(&next_word, &next_word_len, c);
        tab[i] = (char *)malloc(sizeof(char) * (next_word_len + 1));
        if (!tab[i])
            return (ft_malloc_error(tab));
        ft_strlcpy(tab[i], next_word, next_word_len + 1);
        i++;
    }
    tab[i] = NULL;
    return (tab);
}
