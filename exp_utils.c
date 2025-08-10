#include "minishell.h"

char	*extract_key(char *token_val)
{
	int		l;
	int		i;
	char	*key_to_search;

	l = 0;
	i = -1;
	while (token_val[l] && !ft_isspace(token_val[l])
		&& token_val[l] != '"' && token_val[l] != '\''
		&& token_val[l] != '$')
		l++;
	key_to_search = malloc(sizeof(char) * (l + 1));
	if (!key_to_search)
		return (NULL);
	while (++i < l)
		key_to_search[i] = token_val[i];
	key_to_search[l] = '\0';
	return (key_to_search);
}

char	*get_value_of_key(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

static void	append_value(char *exp_value, char *value, int *a_index)
{
	int	i;

	i = 0;
	if (!value)
		return ;
	while (value[i] != '\0')
		exp_value[(*a_index)++] = value[i++];
}

void	expand_token_value(t_data *data, t_token *token, t_exp *node, int i)
{
	char	*key;
	char	*value;
	int		a_index;

	a_index = 0;
	key = NULL;
	value = NULL;
	while (token->value[i] != '\0')
	{
		if (token->value[i] == '$')
		{
			i++;
			key = extract_key(&token->value[i]);
			if (key[0] == '\0')
				node->exp_value[a_index++] = '$';
			i += ft_strlen(key);
			value = get_value_of_key(data->env, key);
			append_value(node->exp_value, value, &a_index);
			free(key);
		}
		else
			node->exp_value[a_index++] = token->value[i++];
	}
	node->exp_value[a_index] = '\0';
}