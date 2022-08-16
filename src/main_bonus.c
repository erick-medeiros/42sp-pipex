/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre-f <eandre-f@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/12 11:24:08 by eandre-f          #+#    #+#             */
/*   Updated: 2022/08/16 10:56:52 by eandre-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "pipex_bonus.h"

// ./pipex file1 cmd1 cmd2 cmd3 ... cmdn file2
// ./pipex here_doc LIMITER cmd cmd1 file

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	if (check_args(&pipex, argc, argv) == 1)
	{
		error(ERR_ARG, NULL);
		exit(1);
	}
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
	if (pipex->here_doc == 0)
	{
		pipex->cmd_start = 3;
		pipex->infile = pipex_here_doc(pipex, argv[2]);
		pipex->outfile = pipex_open(argv[argc - 1], APPEND_MODE);
	}
	else
	{
		pipex->cmd_start = 2;
		pipex->infile = pipex_open(argv[1], IN_MODE);
		pipex->outfile = pipex_open(argv[argc - 1], OUT_MODE);
	}
	if (pipex->infile < 0)
		error(ERR_INFILE, NULL);
	if (pipex->outfile < 0)
		error(ERR_OUTFILE, NULL);
	pipex->cmd_number = argc - pipex->cmd_start - 1;
}
