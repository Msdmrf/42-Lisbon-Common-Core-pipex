/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 10:22:46 by migusant          #+#    #+#             */
/*   Updated: 2025/08/04 21:05:55 by migusant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	if (argc < 5)
	{
		ft_putstr_fd("Error: Bad arguments\n", 2);
		ft_putstr_fd("Usage: ./pipex file1 cmd1 cmd2 file2\n", 2);
		ft_putstr_fd("   or: ./pipex here_doc LIMITER cmd1 cmd2 file2\n", 2);
		return (1);
	}
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
	{
		if (argc < 6)
		{
			ft_putstr_fd("Usage: ./pipex here_doc LIMITER cmd1 cmd2 file\n", 2);
			return (1);
		}
		return (handle_here_doc(argc, argv, envp));
	}
	else if (argc == 5)
	{
		return (handle_basic_pipex(argv, envp));
	}
	else
	{
		return (handle_multiple_pipes(argc, argv, envp));
	}
}
