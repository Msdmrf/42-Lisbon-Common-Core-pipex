/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 11:23:38 by migusant          #+#    #+#             */
/*   Updated: 2025/08/04 21:05:44 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	read_here_doc(char *limiter, int fd)
{
	int		limiter_len;
	char	*line;

	limiter_len = ft_strlen(limiter);
	while (1)
	{
		line = get_next_line(0);
		if (line == NULL)
			break ;
		if (ft_strncmp(line, limiter, limiter_len) == 0
			&& (line[limiter_len] == '\n' || line[limiter_len] == '\0'))
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, fd);
		free(line);
	}
}

pid_t	create_here_doc_writer(char *limiter, t_here_doc_fds *fds)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		error_exit("Fork");
	if (pid == 0)
	{
		close(fds->pipe2_read);
		close(fds->pipe2_write);
		close(fds->outfile);
		read_here_doc(limiter, fds->pipe1_write);
		close(fds->pipe1_write);
		exit(0);
	}
	return (pid);
}

pid_t	create_first_cmd_process(char *cmd, char **envp, t_here_doc_fds *fds)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		error_exit("Fork");
	if (pid == 0)
	{
		close(fds->pipe1_write);
		close(fds->pipe2_read);
		close(fds->outfile);
		dup2(fds->pipe1_read, STDIN_FILENO);
		dup2(fds->pipe2_write, STDOUT_FILENO);
		close(fds->pipe1_read);
		close(fds->pipe2_write);
		execute(cmd, envp);
	}
	return (pid);
}

pid_t	create_second_cmd_process(char *cmd, char **envp, t_here_doc_fds *fds)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		error_exit("Fork");
	if (pid == 0)
	{
		close(fds->pipe1_read);
		close(fds->pipe1_write);
		close(fds->pipe2_write);
		dup2(fds->pipe2_read, STDIN_FILENO);
		dup2(fds->outfile, STDOUT_FILENO);
		close(fds->pipe2_read);
		close(fds->outfile);
		execute(cmd, envp);
	}
	return (pid);
}

int	wait_here_doc_processes(pid_t *pids)
{
	int	status;
	int	i;
	int	exit_code;

	i = 0;
	exit_code = 0;
	while (i < 3)
	{
		waitpid(pids[i], &status, 0);
		if (i == 2 && WIFEXITED(status))
			exit_code = WEXITSTATUS(status);
		i++;
	}
	return (exit_code);
}
