/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre-f <eandre-f@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 15:08:55 by eandre-f          #+#    #+#             */
/*   Updated: 2022/08/11 16:51:54 by eandre-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error(char *err, char *desc)
{
	write(STDERR, err, ft_strlen(err));
	if (desc != NULL)
	{
		write(STDERR, ": ", 2);
		write(STDERR, desc, ft_strlen(desc));
	}
	write(STDERR, "\n", 1);
}

void	error_exit(int status, char *err, char *desc)
{
	error(err, desc);
	exit(status);
}

void	close_pipes(t_pipex *pipex)
{
	close(pipex->pipefd[0]);
	close(pipex->pipefd[1]);
}

void	free_cmd(t_cmd *cmd)
{
	int	i;

	i = -1;
	if (cmd->args != NULL)
	{
		while (cmd->args[++i] != NULL)
			free(cmd->args[i]);
		free(cmd->args);
	}
	free(cmd->runpath);
}

void	free_pipex(t_pipex *pipex)
{
	free_cmd(&pipex->cmd1);
	free_cmd(&pipex->cmd2);
}
