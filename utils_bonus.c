/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 23:34:35 by mzutter           #+#    #+#             */
/*   Updated: 2025/02/25 00:54:44 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void ft_error(const char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
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

static char *trim_quotes(char *str)
{
    int start = 0;
    int end = ft_strlen(str) - 1;

    if (str[start] == '"' || str[start] == '\'')
        start++;  // Skip the first quote
    if (str[end] == '"' || str[end] == '\'')
        end--;  // Skip the last quote

    // Create a new string with the trimmed content
    char *trimmed = malloc(end - start + 2);  // +1 for null terminator
    if (!trimmed)
        return NULL;
    
    // Copy the trimmed string
    ft_strlcpy(trimmed, &str[start], end - start + 1);
    trimmed[end - start + 1] = '\0';  // Null-terminate the string

    return trimmed;
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


