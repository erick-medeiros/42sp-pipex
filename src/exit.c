/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre-f <eandre-f@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 15:08:55 by eandre-f          #+#    #+#             */
/*   Updated: 2022/08/16 23:40:00 by eandre-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error(int status, char *desc)
{
	if (status == 127)
	{
		write(STDERR, desc, ft_strlen(desc));
		write(STDERR, ": ", 2);
		write(STDERR, ERR_CMD, ft_strlen(ERR_CMD));
		write(STDERR, "\n", 1);
	}
	else
		strerror(status);
}

void	error_exit(t_pipex *pipex, int status, char *msg_err)
{
	write(STDERR, msg_err, ft_strlen(msg_err));
	write(STDERR, "\n", 1);
	free_pipex(pipex);
	exit(status);
}

void	arg_error(int status, char *msg_err)
{
	write(STDERR, msg_err, ft_strlen(msg_err));
	write(STDERR, "\n", 1);
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
	if (access(PATH_TEMP_FILE, F_OK) == 0)
		unlink(PATH_TEMP_FILE);
}
