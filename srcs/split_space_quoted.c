/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_space_quoted.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrimah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 16:03:51 by yrimah            #+#    #+#             */
/*   Updated: 2023/04/18 16:47:51 by yrimah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/minishell.h"

int	ft_number_of_words(const char *str, char *delim)
{
	int	count;
	int	fnd_quote;

	count = 0;
	fnd_quote = 0;
	while (*str)
	{
		if (*str == '\"' || *str == '\'')
		{
			if (fnd_quote == *str)
				fnd_quote = 0;
			else if (!fnd_quote)
				fnd_quote = *str;
		}
		else if (fnd_quote == 0 && strchr(delim, *str))
			count++;
		str++;
	}
	if (fnd_quote)
		return (-1);
	return (count + 1);
}

// static char	**ft_fill_words(char **aux, char const *str,
// 		char *delim, int i[3])
// {
// 	int	start;
// 	int	end;
// 	int	fnd_quotes;

// 	while (str[i[0]])
// 	{
// 		fnd_quotes = 0;
// 		while (str[i[0]] && ft_strchr(delim, str[i[0]]))
// 			i[0]++;
// 		start = i[0];
// 		while (str[i[0]])
// 		{
// 			if (str[i[0]] == '\'' || str[i[0]] == '\"')
// 			{
// 				if (fnd_quotes == str[i[0]])
// 					fnd_quotes = 0;
// 				else if (!fnd_quotes)
// 					fnd_quotes = str[i[0]];
// 			}
// 			else if (!fnd_quotes && ft_strchr(delim, str[i[0]]))
// 				break ;
// 			i[0]++;
// 		}
// 		end = i[0];
// 		aux[i[2]++] = ft_substr(str, start, end - start);
// 		// i[2]++;
// 	}
// 	aux[i[2]] = NULL;
// 	return (aux);
// }

static int	ft_find_word_end(char const *str, char *delim,
								int start, int *quotes)
{
	int	i;

	i = start;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			if (*quotes == str[i])
				*quotes = 0;
			else if (!*quotes)
				*quotes = str[i];
		}
		else if (!*quotes && ft_strchr(delim, str[i]))
			break ;
		i++;
	}
	return (i);
}

static char	**ft_fill_words(char **aux, char const *str, char *delim, int i[3])
{
	int	start;
	int	end;
	int	quotes;

	while (str[i[0]])
	{
		quotes = 0;
		while (str[i[0]] && ft_strchr(delim, str[i[0]]))
			i[0]++;
		start = i[0];
		end = ft_find_word_end(str, delim, start, &quotes);
		aux[i[2]++] = ft_substr(str, start, end - start);
		i[0] = end;
	}
	aux[i[2]] = NULL;
	return (aux);
}

// static char	**ft_fill_words(char **aux, const char *str, char *delim, int i[3])
// {
// 	int	fnd_quotes;

// 	while (str[i[0]])
// 	{
// 		fnd_quotes = 0;
// 		while (str[i[0]] && ft_strchr(delim, str[i[0]]))
// 			i[0]++;
// 		if (str[i[0]] == '\0')
// 			break ;
// 		i[1] = i[0];
// 		while (str[i[0]] && (fnd_quotes || !ft_strchr(delim, str[i[0]])))
// 		{
// 			if (str[i[0]] == '\'' || str[i[0]] == '\"')
// 				fnd_quotes = (fnd_quotes == str[i[0]]) ? 0 : str[i[0]];
// 			i[0]++;
// 		}
// 		aux[i[2]++] = ft_substr(str, i[1], i[0] - i[1]);
// 	}
// 	aux[i[2]] = NULL;
// 	return (aux);
// }

// static char **ft_fill_words(char **aux, char const *str, char *delim, int i[3])
// {
//     int start;
//     int end;
//     int in_quotes;
    
//     while (str[i[0]])
//     {
//         in_quotes = 0;
//         while (str[i[0]] && ft_strchr(delim, str[i[0]]))
//             i[0]++;
//         start = i[0];
//         while (str[i[0]])
//         {
//             if (str[i[0]] == '\'' || str[i[0]] == '\"')
//             {
//                 if (!in_quotes)
//                     in_quotes = str[i[0]];
//                 else if (in_quotes == str[i[0]])
//                     in_quotes = 0;
//             }
//             else if (!in_quotes && ft_strchr(delim, str[i[0]]))
//                 break;
//             i[0]++;
//         }
//         end = i[0];
//         aux[i[2]] = ft_substr(str, start, end - start);
//         i[2]++;
//     }
//     aux[i[2]] = NULL;
//     return (aux);
// }


char	**ft_split_space_quoted(char const *str, char *delim)
{
	char	**trimed;
	int		nbr_words;
	int		j;
	int		i[3];

	j = 0;
	while (j <= 2)
		i[j++] = 0;
	if (!str)
		return (NULL);
	nbr_words = ft_number_of_words(str, delim);
	if (nbr_words == -1)
		return (NULL);
	trimed = malloc((nbr_words + 1) * sizeof(char *));
	if (trimed == NULL)
		return (NULL);
	trimed = ft_fill_words(trimed, str, delim, i);
	trimed[nbr_words] = NULL;
	return (trimed);
}
// function test //

// int main()
// {
//     const char str[] =
//     "echo \"hello      there\" how are 'you 'doing? $USER |wc -l >outfile  '   tehre hrehe '";
//     char **s;
//     int i = 0;
//     s = ft_split_space_quoted(str, " ");
//     while (s[i])
//     {
//         printf("%d = %s\n", i, s[i]);
//         i++;
//     }
//     if (!s[i])
//         printf("OK");
//     return (0);
// }
