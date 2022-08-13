/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre-f <eandre-f@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 15:08:55 by eandre-f          #+#    #+#             */
/*   Updated: 2022/08/13 18:02:13 by eandre-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

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

void	free_error_exit(t_pipex *pipex, int status, char *err, char *desc)
{
	error(err, desc);
	free_pipex(pipex);
	exit(status);
}

void	close_pipes(t_pipex *pipex)
{
	close(pipex->pipefd1[0]);
	close(pipex->pipefd1[1]);
	close(pipex->pipefd2[0]);
	close(pipex->pipefd2[1]);
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
	size_t	i;

	if (pipex->cmd != NULL)
	{
		i = -1;
		while (pipex->cmd[++i] != NULL)
		{
			free_cmd(pipex->cmd[i]);
			free(pipex->cmd[i]);
		}
		free(pipex->cmd[i]);
		free(pipex->cmd);
	}
	if (pipex->paths != NULL)
	{
		i = -1;
		while (pipex->paths[++i] != NULL)
			free(pipex->paths[i]);
		free(pipex->paths);
	}
	close(pipex->infile);
	close(pipex->outfile);
}
