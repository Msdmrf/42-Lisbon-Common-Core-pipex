/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_pipes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 10:25:38 by migusant          #+#    #+#             */
/*   Updated: 2025/08/04 21:05:53 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	handle_multiple_pipes(int argc, char **argv, char **envp)
{
	int					**pipes;
	pid_t				*pids;
	int					num_cmds;
	int					exit_code;
	t_mul_pipes_proc	args;

	num_cmds = argc - 3;
	if (num_cmds < 2)
		return (1);
	if (allocate_pipes_and_pids(num_cmds, &pipes, &pids) == -1)
		error_exit("Malloc");
	if (init_pipes(pipes, num_cmds - 1) == -1)
		error_exit("Pipe");
	args.argc = argc;
	args.argv = argv;
	args.envp = envp;
	args.pipes = pipes;
	args.num_cmds = num_cmds;
	if (fork_children(&args, pids) == -1)
		error_exit("Fork");
	close_pipes(pipes, num_cmds - 1);
	exit_code = wait_for_children(pids, num_cmds);
	free_pipes_and_pids(pipes, num_cmds - 1, pids);
	return (exit_code);
}
