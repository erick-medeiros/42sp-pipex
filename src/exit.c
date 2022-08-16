/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre-f <eandre-f@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 15:08:55 by eandre-f          #+#    #+#             */
/*   Updated: 2022/08/16 19:05:35 by eandre-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	child_error(t_cmd *cmd)
{
	if (cmd->status == 127)
	{
		write(STDERR, cmd->desc, ft_strlen(cmd->desc));
		write(STDERR, ": ", 2);
		write(STDERR, ERR_CMD, ft_strlen(ERR_CMD));
		write(STDERR, "\n", 1);
	}
	else
		strerror(cmd->status);
}

void	error_exit(t_pipex *pipex, int status, char *msg_err)
{
	write(STDERR, msg_err, ft_strlen(msg_err));
	write(STDERR, "\n", 1);
	free_pipex(pipex);
	exit(status);
}

void	close_pipes(t_pipex *pipex)
{
	int	i;

	if (pipex->pipefds != NULL)
	{
		i = 0;
		while (pipex->pipefds[i] != NULL)
		{
			close(pipex->pipefds[i][0]);
			close(pipex->pipefds[i][1]);
			++i;
		}
	}
	if (pipex->infile >= 0)
		close(pipex->infile);
	if (pipex->outfile >= 0)
		close(pipex->outfile);
}

void	free_pipex(t_pipex *pipex)
{
	size_t	i;

	if (pipex->cmd != NULL)
	{
		i = -1;
		while (pipex->cmd[++i] != NULL)
		{
			if (pipex->cmd[i]->args != NULL)
				ft_free_list((void ***)&pipex->cmd[i]->args);
			free(pipex->cmd[i]->runpath);
			free(pipex->cmd[i]);
		}
		free(pipex->cmd);
	}
	ft_free_list((void ***)&pipex->pipefds);
	ft_free_list((void ***)&pipex->paths);
}
