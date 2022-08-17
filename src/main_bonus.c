/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre-f <eandre-f@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/12 11:24:08 by eandre-f          #+#    #+#             */
/*   Updated: 2022/08/16 23:39:35 by eandre-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "pipex_bonus.h"

// ./pipex file1 cmd1 cmd2 cmd3 ... cmdn file2
// ./pipex here_doc LIMITER cmd cmd1 file

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	if (argc < 5)
		arg_error(1, ERR_ARG);
	pipex.here_doc = ft_strcmp(argv[1], "here_doc");
	if (argc < 6 && pipex.here_doc == 0)
		arg_error(1, ERR_ARG);
	pipex_io(&pipex, argc, argv);
	pipex_init(&pipex, envp);
	pipex_commands(&pipex, argv);
	pipex_tubing(&pipex);
	free_pipex(&pipex);
	exit(pipex.exit_status);
}

void	pipex_io(t_pipex *pipex, int argc, char **argv)
{
	if (pipex->here_doc == 0)
	{
		pipex->cmd_start = 3;
		pipex->infile = pipex_here_doc(pipex, argv[2]);
		if (pipex->infile < 0)
			perror(ERR_HERE_DOC);
		pipex->outfile = pipex_open(argv[argc - 1], APPEND_MODE);
		if (pipex->outfile < 0)
			perror(argv[argc - 1]);
	}
	else
	{
		pipex->cmd_start = 2;
		pipex->infile = pipex_open(argv[1], IN_MODE);
		if (pipex->infile < 0)
			perror(argv[1]);
		pipex->outfile = pipex_open(argv[argc - 1], OUT_MODE);
		if (pipex->outfile < 0)
			perror(argv[argc - 1]);
	}
	pipex->cmd_number = argc - pipex->cmd_start - 1;
}
