/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre-f <eandre-f@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/11 16:38:37 by eandre-f          #+#    #+#             */
/*   Updated: 2022/08/12 11:41:09 by eandre-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
	if (execve(cmd->runpath, cmd->args, cmd->envp) == -1)
		process_exit(pipex, 1);
	process_exit(pipex, 0);
}
