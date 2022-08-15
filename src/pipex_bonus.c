/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre-f <eandre-f@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/09 15:08:22 by eandre-f          #+#    #+#             */
/*   Updated: 2022/08/15 14:38:47 by eandre-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	pipex_init(t_pipex *pipex, int argc, char **argv, char **envp)
{
	get_paths(pipex, envp);
	pipex->envp = envp;
	pipex->pipefds = NULL;
	pipex->cmd = NULL;
	if (pipex->here_doc == 0)
	{
		pipex->infile = pipex_here_doc(pipex, argv[2]);
		pipex->outfile = pipex_open(argv[argc - 1], APPEND_MODE);
	}
	else
	{
		pipex->infile = pipex_open(argv[1], IN_MODE);
		pipex->outfile = pipex_open(argv[argc - 1], OUT_MODE);
	}
	if (pipex->infile < 0)
		error(ERR_INFILE, NULL);
	if (pipex->outfile < 0)
		error(ERR_OUTFILE, NULL);
	pipex->exit_status = 0;
}

void	pipex_tubing(t_pipex *pipex)
{
	int	i;

	i = -1;
	while (++i < pipex->cmd_number)
	{
		pipex->cmd[i]->pid = fork();
		define_stds(pipex, i);
		if (pipex->cmd[i]->pid == 0)
			child_process(pipex, pipex->cmd[i]);
	}
	close_pipes(pipex);
	i = -1;
	while (++i < pipex->cmd_number)
	{
		waitpid(pipex->cmd[i]->pid, &pipex->cmd[i]->status, 0);
		if (WIFEXITED(pipex->cmd[i]->status))
			pipex->cmd[i]->status = WEXITSTATUS(pipex->cmd[i]->status);
	}
	pipex->exit_status = pipex->cmd[pipex->cmd_number - 1]->status;
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
