/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 17:51:35 by migusant          #+#    #+#             */
/*   Updated: 2025/08/04 21:05:41 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error_exit(char *error)
{
	perror(error);
	exit(EXIT_FAILURE);
}

void	handle_file_error(char *filename, int *fd)
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
	exit(1);
}

void	handle_dup_error(char *error, int infile, int *fd)
{
	if (infile >= 0)
		close(infile);
	close(fd[0]);
	close(fd[1]);
	error_exit(error);
}
