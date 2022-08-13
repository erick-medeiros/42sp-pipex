/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre-f <eandre-f@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/12 11:24:08 by eandre-f          #+#    #+#             */
/*   Updated: 2022/08/12 23:11:03 by eandre-f         ###   ########.fr       */
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
		pipex_init(&pipex, argc, argv, envp);
		pipex_cmd(&pipex, &pipex.cmd1, argv[2]);
		if (!pipex.cmd1.runpath)
			error(ERR_CMD, argv[2]);
		pipex_cmd(&pipex, &pipex.cmd2, argv[3]);
		if (!pipex.cmd2.runpath)
			error(ERR_CMD, argv[3]);
		pipex_tubing(&pipex);
		free_pipex(&pipex);
		exit(pipex.exit_status);
	}
	return (0);
}
