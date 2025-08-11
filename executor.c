/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sude <sude@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 19:40:29 by sude              #+#    #+#             */
/*   Updated: 2025/08/11 17:11:35 by sude             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

void	apply_redirections(t_redirection *redir)
{
	int	fd;

	while (redir)
	{
		if (redir->type == INPUT) // <
		{
			fd = open(redir->filename, O_RDONLY);
			if (fd < 0)
			{
				perror("open");
				exit(1);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (redir->type == OUTPUT) // >
		{
			fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
			{
				perror("open");
				exit(1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (redir->type == APPEND) // >>
		{
			fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd < 0)
			{
				perror("open");
				exit(1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (redir->type == HEREDOC) // <<
		{
			fd = open(redir->filename, O_RDONLY);
			if (fd < 0)
			{
				printf("minishell: %s: No such file or directory\n", redir->filename);
				perror("open");
				exit(1);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		redir = redir->next;
	}
}

char	*ft_strcpy(char *dest, const char *src)
{
	int	i;

	i = 0;
	if (!dest || !src)
		return (NULL);
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strcat(char *dest, const char *src)
{
	int	dest_len;
	int	i;

	if (!dest || !src)
		return (dest);
	dest_len = ft_strlen(dest);
	i = 0;
	while (src[i])
	{
		dest[dest_len + i] = src[i];
		i++;
	}
	dest[dest_len + i] = '\0';
	return (dest);
}

void	free_array(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
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

void	execute_builtin(t_data *data, char **args)
{
	if (!args)
		return ;
	if (ft_strcmp(args[0], "pwd") == 0)
		data->last_exit_status = pwd_builtin();
	else if (ft_strcmp(args[0], "cd") == 0)
		data->last_exit_status = cd_builtin(args, &data->char_env);
	else if (ft_strcmp(args[0], "echo") == 0)
		data->last_exit_status = echo_builtin(args);
	else if (ft_strcmp(args[0], "export") == 0)
		export_builtin(data, args);
	else if (ft_strcmp(args[0], "unset") == 0)
		unset_builtin(data, args);
	else if (ft_strcmp(args[0], "env") == 0)
		env_builtin(data, args);
	else if (ft_strcmp(args[0], "exit") == 0)
		exit_builtin(data, args);
	return ;
}

char	*create_full_path(char *dir, char *cmd)
{
	char	*full_path;
	int		len;

	len = ft_strlen(dir) + ft_strlen(cmd) + 2;
	full_path = malloc(sizeof(char) * len);
	if (!full_path)
		return (NULL);
	ft_strcpy(full_path, dir);
	ft_strcat(full_path, "/");
	ft_strcat(full_path, cmd);
	return (full_path);
}

char	*find_command_path(char *cmd, char **env)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	int		i;

	path_env = NULL;
	if (access(cmd, F_OK) == 0)
		return (ft_strdup(cmd));
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			path_env = env[i] + 5;
			break ;
		}
		i++;
	}
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = create_full_path(paths[i], cmd);
		if (access(full_path, F_OK) == 0)
		{
			free_array(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_array(paths);
	return (NULL);
}

int	lstsize(t_env *env)
{
	int i;

	i = 0;
	while (env)
	{
		env = env->next;
		i++;
	}
	return (i);
}

char	*ft_strjoin_three(char const *s1, char const *s2, char const *s3)
{
	char	*new_str;
	size_t	i;

	if (!s1 || !s2 || !s3)
		return (NULL);
	i = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3);
	new_str = malloc(sizeof(char) * (i + 1));
	if (!new_str)
		return (NULL);
	ft_strlcpy(new_str, s1, i);
	ft_strlcat(new_str, s2, i);
	ft_strlcat(new_str, s3, i);
	return (new_str);
}

char	**convert_env_to_array(t_env *env)
{
	char	**env_array;
	t_env	*current;
	int		i;

	env_array = malloc(sizeof(char *) * (lstsize(env) + 1));
	if (!env_array)
		return (NULL);
	current = env;
	i = 0;
	while (current)
	{
		if (current->value)
		{
			env_array[i] = ft_strjoin_three(current->key, "=", current->value);
			if (!env_array[i])
			{
				free_array(env_array);
				return (NULL);
			}
			i++;
		}
		current = current->next;
	}
	env_array[i] = NULL;
	return (env_array);
}

void	setup_child_signals()
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}


void execute_command_in_child(t_data *data, char **args)
{
	char *path;
	char **temp_env;

	temp_env = convert_env_to_array(data->env);
	if (!temp_env)
	{
		free_all(data);
		exit(1);
	}
	path = find_command_path(args[0], temp_env);
	if (!path)
	{
		printf("minishell: %s: command not found\n", args[0]);
		free_array(temp_env);
		free_all(data);
		exit(127);
	}
	if (execve(path, args, data->char_env) == -1)
	{
		perror("execve");
		free(path);
		free_array(temp_env);
		free_all(data);
		exit(127);
	}
}

void    handle_error_and_exit(char *cmd, const char *msg, int exit_code)
{
	if(cmd != NULL)
		printf("minishell: %s %s", cmd, msg);
	else 
    	printf("minishell: %s", msg);
    exit(exit_code);
}

void pre_file_check(char *cmd, int *exit)
{
	struct stat st;

	if (cmd && ft_strchr(cmd, '/'))
	{
		if (cmd && stat(cmd, &st) == 0)
		{
			if (S_ISDIR(st.st_mode))
				handle_error_and_exit(NULL, ": Is a directory\n", 126);
			else if (access(cmd, X_OK) == -1)
				handle_error_and_exit(NULL, ": Permission denied\n", 126);
		}
		else
			handle_error_and_exit(NULL, ": No such file or directory\n", 127);
	}
	else if (cmd && access(cmd, F_OK) == 0)
		handle_error_and_exit(cmd, "command not found\n", 127);
	else
		*exit = 0;
}

void child_process(t_data *data, t_parser *cmd, int *pipe_fds, int prev_pipe)
{
	pre_file_check(cmd->args[0], &data->last_exit_status);
	if (data->last_exit_status != 0)
		return ;
	setup_child_signals();
	if (prev_pipe != STDIN_FILENO)
	{
		dup2(prev_pipe, STDIN_FILENO);//ilk komut değilse stdini prev_pipe_read_fd ye yönlendir
		close(prev_pipe);
	}
	if (cmd->next)
	{
		dup2(pipe_fds[1], STDOUT_FILENO);//sonraki komut varsa stdoutu yazma ucuna yönlendri
		close(pipe_fds[0]);
		close(pipe_fds[1]);
	}
	if (cmd->redirection)
			apply_redirections(cmd->redirection);
	if (is_builtin(cmd->args[0]))
	{
		execute_builtin(data, cmd->args);
		free_all(data);
		exit(0);
	}
	else
		execute_command_in_child(data, cmd->args);
}

void	parent_process(int *pipe_fds, int *prev_pipe, t_parser *cmd)
{
    signal(SIGINT, SIG_IGN);
	if (*prev_pipe != STDIN_FILENO)
		close(*prev_pipe);//önceki pipe okuma ucu kapatılır (artık kullanılmayacak)
	if (cmd->next)
	{
		close(pipe_fds[1]);//Pipe ın yazma ucu kapatılır çünkü ana processde yazam işlemi yapılmayacak
		*prev_pipe = pipe_fds[0];// Pipe ın okuma ucu sonraki komut için stdin olarak kullanılacak
		//prev_fd kullanma sebebim bu değişken olmasa pipe fdlerinin karışabileceği
	}
}

void	handle_fork_error(t_parser *cmds, int *pipe_fds, int *prev_read_fd)
{
	perror("minishell: fork error");
	if (*prev_read_fd != STDIN_FILENO)
		close(*prev_read_fd);
	if (cmds->next)
	{
		close(pipe_fds[0]);
		close(pipe_fds[1]);
	}
}
/*
pipe okuma ucu pipeın öncesindeki komutun girdisini almak için kullanılır --->dup2(pipe_fds[0], STDIN_FILENO).
yazma ucu ise pipein öncesindeki komutun çıktısını pipe a aktarmasını sağlar--->dup2(pipe_fds[1], STDOUT_FILENO).
pipeta dup2(pipe_fds[1], STDOUT_FILENO). komutun çıktısını pipe a yönlendirirken(artık pipe a yazar) 
dup2(pipe_fds[0], STDIN_FILENO). 2. komut için stdin i read ucuna yönlendirir(artık pipe dan okur)
*/

static void	handle_waiting(t_data *data, pid_t last_pid)
{
	int	status;

	if (last_pid != -1)
	{
		waitpid(last_pid, &status, 0);
		if (WIFEXITED(status))
			data->last_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			data->last_exit_status = 128 + WTERMSIG(status);
	}
	while (wait(NULL) > 0)
		;
}


void	executor(t_data *data)
{
	t_parser	*cmds;
	int			pipe_fds[2];
	int			prev_pipe_read_fd;
	pid_t		last_pid = -1;

	prev_pipe_read_fd = STDIN_FILENO;
	cmds = data->parser;
	handle_heredoc(data);
	if (!cmds)
		return ;
	if (!cmds->next && is_builtin(cmds->args[0]) && !cmds->redirection)
	{
		execute_builtin(data, cmds->args);
		return ;
	}
	while (cmds)
	{
		if (cmds->next)
		{
			if (pipe(pipe_fds) == -1)
			{
				perror("minishell: pipe error");
				if (prev_pipe_read_fd != STDIN_FILENO)
					close(prev_pipe_read_fd);
				return ;
			}
		}
		last_pid = fork();
		if (last_pid < 0)
		{
			handle_fork_error(cmds, pipe_fds, &prev_pipe_read_fd);
			return ;
		}
		else if (last_pid == 0)
			child_process(data, cmds, pipe_fds, prev_pipe_read_fd);//prev pipe kopyası ile çaşır *prev_pipe_ değil
		else
			parent_process(pipe_fds, &prev_pipe_read_fd, cmds);
		cmds = cmds->next;
	}
	if (prev_pipe_read_fd != STDIN_FILENO)
		close(prev_pipe_read_fd);
	handle_waiting(data, last_pid);
}
