/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 17:31:11 by migusant          #+#    #+#             */
/*   Updated: 2025/11/16 16:23:23 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	if (argc != 5)
	{
		ft_putstr_fd("Error:  Bad arguments\n", 2);
		ft_putstr_fd("Usage:  ./pipex file1 cmd1 cmd2 file2\n", 2);
		ft_putstr_fd("Sample: ./pipex infile \"ls -l\" \"wc -l\" outfile\n", 2);
		return (EXIT_FAILURE);
	}
	return (basic_pipex(argv, envp));
}
