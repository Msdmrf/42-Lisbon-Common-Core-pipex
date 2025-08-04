/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_handlers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 17:37:11 by migusant          #+#    #+#             */
/*   Updated: 2025/08/04 21:06:02 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	first_process(char *infile, char *cmd, char **envp, int *fd)
{
	int	input_fd;

	input_fd = open(infile, O_RDONLY);
	if (input_fd == -1)
		handle_file_error(infile, fd);
	if (dup2(fd[1], STDOUT_FILENO) == -1)
		handle_dup_error("dup2", input_fd, fd);
	if (dup2(input_fd, STDIN_FILENO) == -1)
		handle_dup_error("dup2", input_fd, fd);
	close(fd[0]);
	close(fd[1]);
	close(input_fd);
	execute(cmd, envp);
}

void	last_process(char *cmd, char *outfile, char **envp, int *fd)
{
	int	output_fd;

	output_fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (output_fd == -1)
		handle_file_error(outfile, fd);
	if (dup2(fd[0], STDIN_FILENO) == -1)
		handle_dup_error("dup2", output_fd, fd);
	if (dup2(output_fd, STDOUT_FILENO) == -1)
		handle_dup_error("dup2", output_fd, fd);
	close(fd[0]);
	close(fd[1]);
	close(output_fd);
	execute(cmd, envp);
}

int	wait_for_processes(pid_t pid1, pid_t pid2)
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

int	handle_basic_pipex(char **argv, char **envp)
{
	int		fd[2];
	pid_t	pid1;
	pid_t	pid2;

	if (pipe(fd) == -1)
		error_exit("Pipe");
	pid1 = fork();
	if (pid1 == -1)
		error_exit("Fork");
	if (pid1 == 0)
		first_process(argv[1], argv[2], envp, fd);
	pid2 = fork();
	if (pid2 == -1)
		error_exit("Fork");
	if (pid2 == 0)
		last_process(argv[3], argv[4], envp, fd);
	close(fd[0]);
	close(fd[1]);
	return (wait_for_processes(pid1, pid2));
}
