/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 17:31:18 by migusant          #+#    #+#             */
/*   Updated: 2025/08/04 21:06:08 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include "../libft/includes/libft.h"

// Utility Functions (pipex_utils.c)
void	free_all(char **args);
char	*prepare_command(char *argv, char **envp, char ***cmd);
void	execute(char *argv, char **envp);
int		get_exit_status(int status1, int status2);

// Process Handlers Functions (process_handlers.c)
void	first_process(char *infile, char *cmd, char **envp, int *fd);
void	last_process(char *cmd, char *outfile, char **envp, int *fd);
int		wait_for_processes(pid_t pid1, pid_t pid2);
int		handle_basic_pipex(char **argv, char **envp);

// Path Finding Functions (path_finder.c)
char	*search_in_paths(char *cmd, char **paths);
char	*find_path(char *cmd, char **envp);

// Error Handling Functions (error_handling.c)
void	error_exit(char *error);
void	handle_file_error(char *filename, int *fd);
void	handle_dup_error(char *error, int infile, int *fd);

// Bonus Structure 
typedef struct s_here_doc_fds
{
	int	pipe1_read;
	int	pipe1_write;
	int	pipe2_read;
	int	pipe2_write;
	int	outfile;
}	t_here_doc_fds;

// Bonus Functions (here_doc.c)
int		handle_here_doc(int argc, char **argv, char **envp);

// Bonus Functions (here_doc_utils.c)
void	read_here_doc(char *limiter, int fd);
pid_t	create_here_doc_writer(char *limiter, t_here_doc_fds *fds);
pid_t	create_first_cmd_process(char *cmd, char **envp, t_here_doc_fds *fds);
pid_t	create_second_cmd_process(char *cmd, char **envp, t_here_doc_fds *fds);
int		wait_here_doc_processes(pid_t *pids);

// Bonus Structure (multiple_pipes.c)
typedef struct s_mul_pipes_proc
{
	int		argc;
	char	**argv;
	char	**envp;
	int		**pipes;
	int		num_cmds;
}	t_mul_pipes_proc;

// Bonus Functions (multiple_pipes.c)
int		handle_multiple_pipes(int argc, char **argv, char **envp);

// Bonus Functions (multiple_pipes_utils.c)
int		allocate_pipes_and_pids(int num_cmds, int ***pipes, pid_t **pids);
int		init_pipes(int **pipes, int num_pipes);
void	close_pipes(int **pipes, int num_pipes);
void	free_pipes_and_pids(int **pipes, int num_pipes, pid_t *pids);

// Bonus Functions (multiple_pipes_process.c)
void	middle_child(char *cmd, char **envp, int *prev_pipe, int *next_pipe);
void	handle_middle_child(int i, t_mul_pipes_proc *args);
void	create_child_process(int i, t_mul_pipes_proc *args);
int		fork_children(t_mul_pipes_proc *args, pid_t *pids);
int		wait_for_children(pid_t *pids, int num_cmds);

#endif