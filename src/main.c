/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre-f <eandre-f@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/12 11:24:08 by eandre-f          #+#    #+#             */
/*   Updated: 2022/08/12 11:43:34 by eandre-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

//./pipex file1 cmd1 cmd2 file2

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	if (argc != 5)
		error_exit(1, ERR_ARG, NULL);
	else
	{
		pipex.infile = pipex_open(argv[1], IN_MODE);
		if (pipex.infile < 0)
			error(ERR_INFILE, NULL);
		pipex.outfile = pipex_open(argv[4], OUT_MODE);
		if (pipex.outfile < 0)
			error(ERR_OUTFILE, NULL);
		pipex_cmd(&pipex.cmd1, argv[2], envp);
		if (!pipex.cmd1.runpath)
			error(ERR_CMD, argv[2]);
		pipex_cmd(&pipex.cmd2, argv[3], envp);
		if (!pipex.cmd2.runpath)
			error(ERR_CMD, argv[3]);
		pipex_tubing(&pipex);
		close(pipex.infile);
		close(pipex.outfile);
		free_pipex(&pipex);
		exit(pipex.exit_status);
	}
	return (0);
}
