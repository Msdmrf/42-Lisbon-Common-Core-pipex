/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 17:31:18 by migusant          #+#    #+#             */
/*   Updated: 2025/11/16 18:32:57 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

// Libraries
# include <sys/wait.h>
# include "../libft/includes/libft.h"

// Error Code Macros
# define EXIT_CANNOT_EXECUTE 126
# define EXIT_COMMAND_NOT_FOUND 127

// Error Handling Functions (error_handlers.c)
void	basic_error(char *error, int is_exit);
void	file_error(char *filename, int *fd);
void	dup2_error(char *error, int infile, int *fd);
void	exec_error(char *cmd, char *path, char **args);
void	cmd_error(char *cmd, int is_path, char **args);

// Process Handlers Functions (process_handlers.c)
int		basic_pipex(char **argv, char **envp);
void	first_process(char *infile, char *cmd, char **envp, int *fd);
void	last_process(char *cmd, char *outfile, char **envp, int *fd);

// Command Execution Functions (pipex_utils.c)
void	execute_command(char *argv, char **envp);
void	free_all(char **args);

#endif