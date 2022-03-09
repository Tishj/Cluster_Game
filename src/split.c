/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   split.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/09 22:08:45 by tbruinem      #+#    #+#                 */
/*   Updated: 2022/03/09 22:09:54 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <strings.h>

static int		get_size(const char *s, char c)
{
	int		i;
	int		size;
	char	last;

	size = 0;
	i = 0;
	last = c;
	while (s[i])
	{
		if (last == c && s[i] != c)
			size++;
		last = s[i];
		i++;
	}
	if (c == '\0')
		return (1);
	return (size);
}

static int		get_wordlen(const char *s, char c)
{
	int i;

	i = 0;
	while (s[i] != 0 && s[i] != c)
		i++;
	return (i);
}

static char		*get_word(const char *s, char c)
{
	char	*word;
	int		i;

	i = 0;
	word = malloc(sizeof(char) * get_wordlen(s, c) + 1);
	if (word == NULL)
		return (NULL);
	while (s[i] && s[i] != c)
	{
		word[i] = s[i];
		i++;
	}
	word[i] = 0;
	return (word);
}

static	char	**assign_words(char **str, char *s, char c, int j)
{
	char	last;

	last = c;
	while (*s)
	{
		if (last == c && *s != c)
		{
			str[j] = get_word(s, c);
			if (str[j] == NULL)
			{
				while (j >= 0)
				{
					free(str[j]);
					j--;
				}
				free(str);
				return (NULL);
			}
			j++;
		}
		last = *s;
		s++;
	}
	str[j] = 0;
	return (str);
}

char			**split(const char *s, char c)
{
	char	**str;

	if (s == NULL)
		return (NULL);
	str = malloc(sizeof(char *) * get_size(s, c) + 1);
	if (str == NULL)
		return (NULL);
	str = assign_words(str, (char *)s, c, 0);
	if (str == NULL)
		return (NULL);
	return (str);
}
