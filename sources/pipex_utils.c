/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 17:36:59 by migusant          #+#    #+#             */
/*   Updated: 2025/11/16 16:23:31 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	*search_in_paths(char *cmd, char **paths)
{
	char	*path;
	char	*part_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		part_path = ft_strjoin(paths[i], "/");
		if (!part_path)
			return (NULL);
		path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (!path)
			return (NULL);
		if (access(path, F_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	return (NULL);
}

static char	*find_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	int		i;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5))
		i++;
	if (!envp[i])
		return (NULL);
	paths = ft_split(envp[i] + 5, ':');
	if (!paths)
		return (NULL);
	path = search_in_paths(cmd, paths);
	free_all(paths);
	return (path);
}

static char	*prepare_command(char *argv, char **envp, char ***cmd)
{
	char	*path;
	int		has_slash;

	*cmd = ft_split(argv, ' ');
	if (!*cmd || !(*cmd)[0])
		cmd_error("", 0, *cmd);
	path = find_path((*cmd)[0], envp);
	if (!path)
	{
		has_slash = (ft_strchr((*cmd)[0], '/') != NULL);
		cmd_error((*cmd)[0], has_slash, *cmd);
	}
	return (path);
}

void	execute_command(char *argv, char **envp)
{
	char	*path;
	char	**cmd;

	path = prepare_command(argv, envp, &cmd);
	if (execve(path, cmd, envp) == -1)
		exec_error(cmd[0], path, cmd);
}

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
