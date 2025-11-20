/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_handlers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 17:37:11 by migusant          #+#    #+#             */
/*   Updated: 2025/11/17 13:55:15 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	get_exit_status(int status1, int status2)
{
	if (WIFEXITED(status2))
		return (WEXITSTATUS(status2));
	else if (WIFSIGNALED(status2))
		return (128 + WTERMSIG(status2));
	else if (WIFEXITED(status1))
		return (WEXITSTATUS(status1));
	else if (WIFSIGNALED(status1))
		return (128 + WTERMSIG(status1));
	return (EXIT_FAILURE);
}

static int	wait_for_processes(pid_t pid1, pid_t pid2)
{
	int		status[2];
	pid_t	finished_pid;

	finished_pid = wait(&status[0]);
	if (finished_pid == pid1)
		waitpid(pid2, &status[1], 0);
	else
	{
		status[1] = status[0];
		waitpid(pid1, &status[0], 0);
	}
	return (get_exit_status(status[0], status[1]));
}

int	basic_pipex(char **argv, char **envp)
{
	int		fd[2];
	int		outfile_fd;
	pid_t	pid1;
	pid_t	pid2;

	outfile_fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile_fd == -1)
		basic_error(argv[4], 0);
	else
		close (outfile_fd);
	if (pipe(fd) == -1)
		basic_error("pipe", 1);
	pid1 = fork();
	if (pid1 == -1)
		basic_error("fork", 1);
	if (pid1 == 0)
		first_process(argv[1], argv[2], envp, fd);
	pid2 = fork();
	if (pid2 == -1)
		basic_error("fork", 1);
	if (pid2 == 0)
		last_process(argv[3], argv[4], envp, fd);
	close(fd[0]);
	close(fd[1]);
	return (wait_for_processes(pid1, pid2));
}

void	first_process(char *infile, char *cmd, char **envp, int *fd)
{
	int	input_fd;

	input_fd = open(infile, O_RDONLY);
	if (input_fd == -1)
		file_error(infile, fd);
	if (dup2(input_fd, STDIN_FILENO) == -1)
		dup2_error("dup2", input_fd, fd);
	if (dup2(fd[1], STDOUT_FILENO) == -1)
		dup2_error("dup2", input_fd, fd);
	close(fd[0]);
	close(fd[1]);
	close(input_fd);
	execute_command(cmd, envp);
}

void	last_process(char *cmd, char *outfile, char **envp, int *fd)
{
	int	output_fd;

	output_fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (output_fd == -1)
	{
		close(fd[0]);
		close(fd[1]);
		exit(EXIT_FAILURE);
	}
	if (dup2(fd[0], STDIN_FILENO) == -1)
		dup2_error("dup2", output_fd, fd);
	if (dup2(output_fd, STDOUT_FILENO) == -1)
		dup2_error("dup2", output_fd, fd);
	close(fd[0]);
	close(fd[1]);
	close(output_fd);
	execute_command(cmd, envp);
}
