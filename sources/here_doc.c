/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 10:24:50 by migusant          #+#    #+#             */
/*   Updated: 2025/08/04 21:05:49 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	handle_here_doc(int argc, char **argv, char **envp)
{
	int				pipe1[2];
	int				pipe2[2];
	t_here_doc_fds	fds;
	pid_t			pids[3];

	if (pipe(pipe1) == -1)
		error_exit("Pipe");
	if (pipe(pipe2) == -1)
		error_exit("Pipe");
	fds.pipe1_read = pipe1[0];
	fds.pipe1_write = pipe1[1];
	fds.pipe2_read = pipe2[0];
	fds.pipe2_write = pipe2[1];
	fds.outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fds.outfile == -1)
		error_exit("Open outfile");
	pids[0] = create_here_doc_writer(argv[2], &fds);
	pids[1] = create_first_cmd_process(argv[3], envp, &fds);
	pids[2] = create_second_cmd_process(argv[4], envp, &fds);
	close(fds.pipe1_read);
	close(fds.pipe1_write);
	close(fds.pipe2_read);
	close(fds.pipe2_write);
	close(fds.outfile);
	return (wait_here_doc_processes(pids));
}
