/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre-f <eandre-f@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/11 16:38:37 by eandre-f          #+#    #+#             */
/*   Updated: 2022/08/13 18:25:14 by eandre-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

// WIFEXITED

int	macro_wifexited(int status)
{
	return (((status) & 0xff) == 0);
}

// WEXITSTATUS

int	macro_wexitstatus(int status)
{
	return (((status) >> 8) & 0xff);
}

static void	process_exit(t_pipex *pipex, int status)
{
	free_pipex(pipex);
	exit(status);
}

void	define_stds(t_pipex *pipex, int i)
{
	if (pipex->pipe_number == 1)
	{
		pipex->cmd[i]->stdin = pipex->pipefd1[0];
		pipex->cmd[i]->stdout = pipex->pipefd2[1];
	}
	else
	{
		pipex->cmd[i]->stdin = pipex->pipefd2[0];
		pipex->cmd[i]->stdout = pipex->pipefd1[1];
	}
	if (i == 0)
		pipex->cmd[i]->stdin = pipex->infile;
	else if (i == (pipex->cmd_number - 1))
		pipex->cmd[i]->stdout = pipex->outfile;
}

void	child_process(t_pipex *pipex, t_cmd *cmd)
{
	if (cmd->stdin < 0 || cmd->stdout < 0)
	{
		close_pipes(pipex);
		process_exit(pipex, 1);
	}
	dup2(cmd->stdin, STDIN);
	dup2(cmd->stdout, STDOUT);
	close_pipes(pipex);
	if (cmd->runpath == NULL)
		process_exit(pipex, cmd->status);
	if (execve(cmd->runpath, cmd->args, pipex->envp) == -1)
		process_exit(pipex, 1);
	process_exit(pipex, 0);
}
