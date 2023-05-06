#include "../includes/minishell.h"

//just a test file//

char **ft_dup_envp(char **envp);

int main(int argc, char **argv, char **envp)
{
    // char **dup;
    // int count;

    // count = 0;
    // dup = NULL;
    // dup = ft_dup_envp(envp);

    // while (envp && envp[count])
    // {
    //     printf("%s\n", envp[count]);
    //     count++;
    // }
    // printf("\n");
    // count = 0;
    // while (dup && dup[count])
    //     printf("%s\n", dup[count++]);


    char *str;

    str = getcd(NULL, 0);
    printf("%s", str);
    return (0);


}

static char	**ft_fill_words(char **aux, char const *str,
		char *delim, int i[3])
{
	int	start;
	int	end;
	int	fnd_quotes;

	while (str[i[0]])
	{
		fnd_quotes = 0;
        // Find the start index of the next word
		while (str[i[0]] && ft_strchr(delim, str[i[0]]))
			i[0]++;
		start = i[0];
        // Find the end index of the current word
		while (str[i[0]])
		{
			if (str[i[0]] == '\'' || str[i[0]] == '\"')
			{
				if (fnd_quotes == str[i[0]])
					fnd_quotes = 0;
				else if (!fnd_quotes)
					fnd_quotes = str[i[0]];
			}
			else if (!fnd_quotes && ft_strchr(delim, str[i[0]]))
				break ;
			i[0]++;
		}
		end = i[0];
        // Copy the word into the array
		aux[i[2]] = ft_substr(str, start, end - start);
		i[2]++;
	}
    // Set the last element of the array to NULL
	aux[i[2]] = NULL;
	return (aux);
}
