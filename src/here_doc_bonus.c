/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre-f <eandre-f@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 10:46:20 by eandre-f          #+#    #+#             */
/*   Updated: 2022/08/16 18:33:33 by eandre-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "pipex_bonus.h"

int	pipex_here_doc(t_pipex *pipex, char	*limiter)
{
	int	pipefd[2];
	int	pid;

	if (pipe(pipefd) < 0)
		error_exit(pipex, 1, ERR_PIPE);
	pid = fork();
	if (pid < 0)
		error_exit(pipex, 1, ERR_PIPE);
	if (pid == 0)
	{
		pipex->infile = STDIN;
		pipex->outfile = STDOUT;
		close(pipefd[0]);
		child_here_doc(pipex, pipefd, limiter);
	}
	waitpid(pid, NULL, 0);
	dup2(pipefd[0], STDIN);
	close(pipefd[0]);
	close(pipefd[1]);
	return (STDIN);
}

void	child_here_doc(t_pipex *pipex, int fd[2], char	*limiter)
{
	char	*line;
	int		cmplen;

	write(STDIN, MSG_HERE_DOC, ft_strlen(MSG_HERE_DOC));
	line = get_next_line(STDIN);
	while (line)
	{
		cmplen = ft_max(ft_strlen(limiter), ft_strlen(line) - 1);
		if (ft_strncmp(limiter, line, cmplen) == 0)
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
