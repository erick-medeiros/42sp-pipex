/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre-f <eandre-f@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/12 11:24:08 by eandre-f          #+#    #+#             */
/*   Updated: 2022/08/16 11:08:51 by eandre-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// ./pipex file1 cmd1 cmd2 file2

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	if (argc != 5)
		free_error_exit(&pipex, 1, ERR_ARG, NULL);
	else
	{
		pipex_io(&pipex, argc, argv);
		pipex_init(&pipex, envp);
		pipex_commands(&pipex, argv);
		pipex_tubing(&pipex);
		free_pipex(&pipex);
		exit(pipex.exit_status);
	}
	return (0);
}

void	pipex_io(t_pipex *pipex, int argc, char **argv)
{
	pipex->here_doc = -1;
	pipex->cmd_start = 2;
	pipex->cmd_number = argc - pipex->cmd_start - 1;
	pipex->infile = pipex_open(argv[1], IN_MODE);
	if (pipex->infile < 0)
		error(ERR_INFILE, NULL);
	pipex->outfile = pipex_open(argv[argc - 1], OUT_MODE);
	if (pipex->outfile < 0)
		error(ERR_OUTFILE, NULL);
}
