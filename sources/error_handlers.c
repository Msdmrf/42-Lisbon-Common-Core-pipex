/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 17:51:35 by migusant          #+#    #+#             */
/*   Updated: 2025/11/16 18:33:15 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	basic_error(char *error)
{
	ft_putstr_fd("pipex: ", 2);
	ft_putstr_fd(error, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	exit(EXIT_FAILURE);
}

void	file_error(char *filename, int *fd)
{
	if (fd)
	{
		close(fd[0]);
		close(fd[1]);
	}
	ft_putstr_fd("pipex: ", 2);
	ft_putstr_fd(filename, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	exit(EXIT_FAILURE);
}

void	dup2_error(char *error, int infile, int *fd)
{
	if (infile >= 0)
		close(infile);
	close(fd[0]);
	close(fd[1]);
	basic_error(error);
}

void	exec_error(char *cmd, char *path, char **args)
{
	ft_putstr_fd("pipex: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	free(path);
	free_all(args);
	exit(EXIT_CANNOT_EXECUTE);
}

void	cmd_error(char *cmd, int is_path, char **args)
{
	ft_putstr_fd("pipex: ", 2);
	ft_putstr_fd(cmd, 2);
	if (is_path)
		ft_putstr_fd(": No such file or directory\n", 2);
	else
		ft_putstr_fd(": command not found\n", 2);
	free_all(args);
	exit(EXIT_COMMAND_NOT_FOUND);
}
