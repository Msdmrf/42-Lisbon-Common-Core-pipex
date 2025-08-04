/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 17:36:59 by migusant          #+#    #+#             */
/*   Updated: 2025/08/04 21:05:58 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_all(char **args)
{
	int	i;

	i = 0;
	if (!args)
		return ;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

char	*prepare_command(char *argv, char **envp, char ***cmd)
{
	char	*path;

	*cmd = ft_split(argv, ' ');
	if (!*cmd || !(*cmd)[0])
	{
		free_all(*cmd);
		ft_putstr_fd("pipex: Command not found\n", 2);
		exit(127);
	}
	path = find_path((*cmd)[0], envp);
	if (!path)
	{
		ft_putstr_fd("pipex: ", 2);
		ft_putstr_fd((*cmd)[0], 2);
		if (ft_strchr((*cmd)[0], '/'))
			ft_putstr_fd(": No such file or directory\n", 2);
		else
			ft_putstr_fd(": Command not found\n", 2);
		free_all(*cmd);
		exit(127);
	}
	return (path);
}

void	execute(char *argv, char **envp)
{
	char	**cmd;
	char	*path;

	path = prepare_command(argv, envp, &cmd);
	if (execve(path, cmd, envp) == -1)
	{
		if (errno == EACCES)
		{
			ft_putstr_fd("pipex: ", 2);
			ft_putstr_fd(cmd[0], 2);
			ft_putstr_fd(": Permission denied\n", 2);
			free(path);
			free_all(cmd);
			exit(126);
		}
		free(path);
		free_all(cmd);
		error_exit("Execve failed");
	}
}

int	get_exit_status(int status1, int status2)
{
	if (WIFEXITED(status2))
		return (WEXITSTATUS(status2));
	else if (WIFSIGNALED(status2))
		return (128 + WTERMSIG(status2));
	else if (WIFEXITED(status1))
		return (WEXITSTATUS(status1));
	else if (WIFSIGNALED(status1))
		return (128 + WTERMSIG(status1));
	return (0);
}
