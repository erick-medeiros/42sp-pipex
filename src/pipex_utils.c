/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre-f <eandre-f@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/09 15:08:22 by eandre-f          #+#    #+#             */
/*   Updated: 2022/08/15 14:16:18 by eandre-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	pipex_init(t_pipex *pipex, int argc, char **argv, char **envp)
{
	get_paths(pipex, envp);
	pipex->envp = envp;
	pipex->infile = pipex_open(argv[1], IN_MODE);
	if (pipex->infile < 0)
		error(ERR_INFILE, NULL);
	pipex->outfile = pipex_open(argv[argc - 1], OUT_MODE);
	if (pipex->outfile < 0)
		error(ERR_OUTFILE, NULL);
}

void	pipex_tubing(t_pipex *pipex)
{
	if (pipe(pipex->pipefd) < 0)
		error_exit(1, ERR_PIPE, NULL);
	pipex->pid1 = fork();
	if (pipex->pid1 == 0)
	{
		pipex->cmd1.stdin = pipex->infile;
		pipex->cmd1.stdout = pipex->pipefd[1];
		child_process(pipex, &pipex->cmd1);
	}
	pipex->pid2 = fork();
	if (pipex->pid2 == 0)
	{
		pipex->cmd2.stdin = pipex->pipefd[0];
		pipex->cmd2.stdout = pipex->outfile;
		child_process(pipex, &pipex->cmd2);
	}
	close_pipes(pipex);
	waitpid(pipex->pid1, &pipex->cmd1.status, 0);
	waitpid(pipex->pid2, &pipex->cmd2.status, 0);
	if (macro_wifexited(pipex->cmd1.status))
		pipex->cmd1.status = macro_wexitstatus(pipex->cmd1.status);
	if (macro_wifexited(pipex->cmd2.status))
		pipex->cmd2.status = macro_wexitstatus(pipex->cmd2.status);
	pipex->exit_status = pipex->cmd2.status;
}
