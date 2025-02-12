/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 10:54:24 by osivkov           #+#    #+#             */
/*   Updated: 2024/10/18 15:42:40 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_word(const char *s, char ch)
{
	size_t	count;
	int		in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s != ch && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (*s == ch)
		{
			in_word = 0;
		}
		s++;
	}
	return (count);
}

static char	*word_dup(const char *s, size_t start, size_t end)
{
	char	*word;
	size_t	i;

	i = 0;
	word = (char *)malloc((end - start + 1) * sizeof(char));
	if (!word)
		return (NULL);
	while (start < end)
	{
		word[i++] = s[start++];
	}
	word[i] = '\0';
	return (word);
}

static void	free_words(char **result, int j)
{
	while (j > 0)
	{
		free (result[--j]);
	}
	free (result);
}

static int	fill_word(char **result, const char *s, char c)
{
	size_t	i;
	size_t	j;
	int		start;

	start = -1;
	i = 0;
	j = 0;
	while (i <= ft_strlen(s))
	{
		if (s[i] != c && start == -1)
			start = i;
		else if ((s[i] == c || s[i] == '\0') && start != -1)
		{
			result[j] = word_dup(s, start, i);
			if (!result[j])
				return (free_words(result, j), 0);
			j++;
			start = -1;
		}
		i++;
	}
	result[j] = NULL;
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**result;
	size_t	words_count;

	if (!s)
		return (NULL);
	words_count = count_word(s, c);
	result = (char **)malloc((words_count +1) * sizeof(char *));
	if (!result)
		return (NULL);
	if (!fill_word(result, s, c))
	{
		return (NULL);
	}
	return (result);
}

/*int main(void)
{
	char str[] = "hello world      this is C";
	char **result = ft_split(str,' ');

	size_t i = 0;
	while (result[i] != NULL)
	{
		printf("Word %zu: %s\n",i, result[i]);
		free(result[i]);
		i++;
	}
	free(result);
}*/
