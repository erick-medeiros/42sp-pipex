/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre-f <eandre-f@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/09 15:08:22 by eandre-f          #+#    #+#             */
/*   Updated: 2022/08/15 16:00:44 by eandre-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	pipex_init(t_pipex *pipex, int argc, char **argv, char **envp)
{
	get_paths(pipex, envp);
	pipex->here_doc = -1;
	pipex->envp = envp;
	pipex->infile = pipex_open(argv[1], IN_MODE);
	if (pipex->infile < 0)
		error(ERR_INFILE, NULL);
	pipex->outfile = pipex_open(argv[argc - 1], OUT_MODE);
	if (pipex->outfile < 0)
		error(ERR_OUTFILE, NULL);
}
