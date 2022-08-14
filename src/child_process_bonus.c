/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre-f <eandre-f@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/11 16:38:37 by eandre-f          #+#    #+#             */
/*   Updated: 2022/08/13 23:55:30 by eandre-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	process_exit(t_pipex *pipex, int status)
{
	free_pipex(pipex);
	exit(status);
}

void	child_here_doc(t_pipex *pipex, int fd[2], char	*limiter)
{
	char	*line;

	write(STDIN, MSG_HERE_DOC, ft_strlen(MSG_HERE_DOC));
	line = get_next_line(STDIN);
	while (line)
	{
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0)
			break ;
		write(fd[1], line, ft_strlen(line));
		free(line);
		write(STDIN, MSG_HERE_DOC, ft_strlen(MSG_HERE_DOC));
		line = get_next_line(STDIN);
	}
	free(line);
	close(fd[1]);
	free_pipex(pipex);
	exit(0);
}

void	define_stds(t_pipex *pipex, int i)
{
	if (i == 0)
	{
		pipex->cmd[i]->stdin = pipex->infile;
		pipex->cmd[i]->stdout = pipex->pipefds[i][1];
	}
	else if (i == (pipex->cmd_number - 1))
	{
		pipex->cmd[i]->stdin = pipex->pipefds[i - 1][0];
		pipex->cmd[i]->stdout = pipex->outfile;
	}
	else
	{
		pipex->cmd[i]->stdin = pipex->pipefds[i - 1][0];
		pipex->cmd[i]->stdout = pipex->pipefds[i][1];
	}
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
