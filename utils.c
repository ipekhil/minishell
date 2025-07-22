#include "minishell.h"

int	ft_strcmp(char *s1, char *s2)
{
    int i;

    i = 0;
	while ((s1[i] != '\0' || s2[i] != '\0') && s1[i] == s2[i])
        i++;
    return (s1[i] - s2[i]);
}

char	*ft_strcpy(char *dest, const char *src)
{
	int i;

	i = 0;
	while(src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}
char **tokens_to_args(t_token *tokens, int count)
{
	char **args;
	int i;

	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	i = 0;
	while (i < count)
	{
		args[i] = tokens[i].value;
		i++;
	}
	args[i] = NULL;
	return (args);
}

void	execute_builtin(t_data	*data)
{
	char	**args;

	args = tokens_to_args(data->tokens, data->token_count);
	if (!args)
		return ;
	if (ft_strcmp(args[0], "pwd") == 0)
		pwd_builtin();
	else if (ft_strcmp(args[0], "cd") == 0)
		cd_builtin(args, &data->env);
	else if (ft_strcmp(args[0], "echo") == 0)
		echo_builtin(args);
/**
	else if (strcmp(data->tokens[0], "exit") == 0)
		builtin_exit();
	else if (strcmp(data->tokens[0], "env") == 0)
		builtin_env(data->env);
 */
	free(args);
	return ;
}