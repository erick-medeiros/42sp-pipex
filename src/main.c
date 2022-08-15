/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre-f <eandre-f@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/12 11:24:08 by eandre-f          #+#    #+#             */
/*   Updated: 2022/08/15 15:46:55 by eandre-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

//./pipex file1 cmd1 cmd2 file2

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	if (argc != 5)
		free_error_exit(&pipex, 1, ERR_PIPE, NULL);
	else
	{
		pipex_init(&pipex, argc, argv, envp);
		pipex_commands(&pipex, argc, argv);
		pipex.pipefds = ft_calloc(sizeof(int *), pipex.cmd_number);
		pipex.i = -1;
		while (++pipex.i < pipex.cmd_number - 1)
		{
			pipex.pipefds[pipex.i] = malloc(sizeof(int) * 2);
			if (pipe(pipex.pipefds[pipex.i]) < 0)
				free_error_exit(&pipex, 1, ERR_PIPE, NULL);
		}
		pipex_tubing(&pipex);
		free_pipex(&pipex);
		exit(pipex.exit_status);
	}
	return (0);
}
