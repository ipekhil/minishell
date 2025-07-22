#include "minishell.h"

int is_space(char c)
{
	if ((c <= 13 && c >= 9) || c == 32)
		return (1);
	return (0);
}

int is_alpha(char c)
{
	if (c <= 122 && c >= 97)
		return (1);
	return (0);
}

int get_length(char *str)
{
	int i;
	char c;

	i = 0;
	if(str[i] == '"' || str[i] == '\'')
	{
		c = str[i++];
		while(str[i] != c && str[i] != '\0')
			i++;
		if(str[i] == c)
			return(i + 1);
		else
			return (-1);
	}
	while (str[i] && !is_space(str[i]))
		i++;
	return (i);
}

int get_num_of_words(char *str)
{
	int i;
	char c;
	int count;

	i = 0;
	count  = 0;
	while(str[i])
	{	
		while(is_space(str[i]))
			i++;
		c = str[i++];
		if(c == '"' || c == '\'')
		{
			while(str[i] != c && str[i] != '\0')
				i++;
			if(str[i] == c)
				i++;
			else
				return (-1);
			if(is_space(str[i]) || str[i] == '\0')
			{
				i++;
				count +=1;
			}
		}
		else
		{
			while(!is_space(str[i]) && str[i] != '\0')
				i++;
			if(is_space(str[i]) || str[i] == '\0')
				count += 1;
			else
				return -1;
		}
	}
	return (count);
}

int get_words(char *line, char *word)
{
	int i;
	int j;
	char c;

	i = 0;
	j = 0;
	if(line[i] == '"' || line[i] == '\'')
	{
		c = line[i];
		word[j++] = line[i++];
		while(line[i] != c && line[i] != '\0')
		{
			word[j] = line[i];
			j++;
			i++;
		}	
		if(line[i] == c)
			word[j++] = line[i++];
		word[j] = '\0';
	}
	else
	{
		while (!is_space(line[i]) && line[i] != '\0')
			word[j++] = line[i++];
		word[j] = '\0';
	}
	return i;
}

char **without_spaces(t_data *data, int i, int k)
{
	char **words;

	words = malloc(sizeof(char *) * (get_num_of_words(data->line) + 1));
	printf("kelime sayısı: %d\n",get_num_of_words(data->line));
	if(!words)
		return NULL;
	while(data->line[i])
	{
		while(is_space(data->line[i]))
			i++;
		if(data->line[i] && !is_space(data->line[i]))
		{
			if(get_length(&(data->line[i])) == -1)
				return NULL;
			words[k] = malloc(sizeof(char) * (get_length(&(data->line[i])) + 1));
			if(!words[k])
				return NULL;
			i =  i + get_words(&(data->line[i]), words[k]);
			k++;
		}
	}
	words[k] = NULL;
	return(words);
}

void tokenization(t_data *data)
{
    char **words = without_spaces(data, 0, 0);
    int i = 0;
    int count = 0;
    while (words[count])
        count++;
    data->tokens = malloc(sizeof(t_token) * count);
    data->token_count = count;
    for (i = 0; i < count; i++) {
        data->tokens[i].value = words[i];
        data->tokens[i].type = WORD; // veya uygun tip
        data->tokens[i].next = (i < count - 1) ? &data->tokens[i + 1] : NULL;
    }
    free(words); // Eğer words içindeki stringler tokenlara taşındıysa
}
