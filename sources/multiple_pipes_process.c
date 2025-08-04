/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_pipes_process.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 10:44:58 by migusant          #+#    #+#             */
/*   Updated: 2025/08/04 21:05:51 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	middle_child(char *cmd, char **envp, int *prev_pipe, int *next_pipe)
{
	dup2(prev_pipe[0], STDIN_FILENO);
	dup2(next_pipe[1], STDOUT_FILENO);
	close(prev_pipe[0]);
	close(prev_pipe[1]);
	close(next_pipe[0]);
	close(next_pipe[1]);
	execute(cmd, envp);
}

void	handle_middle_child(int i, t_mul_pipes_proc *args)
{
	int	j;

	j = 0;
	while (j < args->num_cmds - 1)
	{
		if (j != i - 1 && j != i)
		{
			close(args->pipes[j][0]);
			close(args->pipes[j][1]);
		}
		j++;
	}
	middle_child(args->argv[i + 2], args->envp, args->pipes[i - 1],
		args->pipes[i]);
}

void	create_child_process(int i, t_mul_pipes_proc *args)
{
	if (i == 0)
	{
		close_pipes(args->pipes + 1, args->num_cmds - 2);
		first_process(args->argv[1], args->argv[2], args->envp, args->pipes[0]);
	}
	else if (i == args->num_cmds - 1)
	{
		close_pipes(args->pipes, args->num_cmds - 2);
		last_process(args->argv[args->argc - 2], args->argv[args->argc - 1],
			args->envp, args->pipes[i - 1]);
	}
	else
	{
		handle_middle_child(i, args);
	}
}

int	fork_children(t_mul_pipes_proc *args, pid_t *pids)
{
	int	i;

	i = 0;
	while (i < args->num_cmds)
	{
		pids[i] = fork();
		if (pids[i] == -1)
			return (-1);
		if (pids[i] == 0)
			create_child_process(i, args);
		i++;
	}
	return (0);
}

int	wait_for_children(pid_t *pids, int num_cmds)
{
	int	i;
	int	status;
	int	exit_code;

	i = 0;
	exit_code = 0;
	while (i < num_cmds)
	{
		waitpid(pids[i], &status, 0);
		if (i == num_cmds - 1 && WIFEXITED(status))
			exit_code = WEXITSTATUS(status);
		i++;
	}
	return (exit_code);
}
