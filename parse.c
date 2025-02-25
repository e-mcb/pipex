#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "pipex.h"
#include "libft/libft.h"

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
    while (s[i] != 0 && s[i] == c)
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
    while (next_word && **next_word == c && !in_quotes)
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

int main(int argc, char **argv)
{
    char *str = argv[1];
    char **result;

    result = ft_split2(str, ' ');
    for (size_t i = 0; result[i] != NULL; i++)
    {
        printf("%s\n", result[i]);
        free(result[i]);
    }
    free(result);

    return 0;
}



