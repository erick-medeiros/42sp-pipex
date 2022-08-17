/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre-f <eandre-f@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 10:46:20 by eandre-f          #+#    #+#             */
/*   Updated: 2022/08/17 16:54:26 by eandre-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "pipex_bonus.h"

static void	child_here_doc(int pipefd[2], char	*limiter);

int	pipex_here_doc(t_pipex *pipex, char	*limiter)
{
	int	pipefd[2];
	int	pid;
	int	fd;

	fd = -1;
	if (pipe(pipefd) < 0)
		error_exit(pipex, 1, MSG_HERE_DOC);
	pid = fork();
	if (pid < 0)
		error_exit(pipex, 1, MSG_HERE_DOC);
	if (pid == 0)
		child_here_doc(pipefd, limiter);
	else
	{
		waitpid(pid, NULL, 0);
		fd = pipex_open(PATH_TEMP_FILE, TEMP_MODE);
		if (fd < 0)
			error_exit(pipex, 1, MSG_HERE_DOC);
		dup2(pipefd[0], fd);
		close(pipefd[0]);
		close(pipefd[1]);
	}
	return (fd);
}

static void	child_here_doc(int pipefd[2], char	*limiter)
{
	char	*line;
	int		cmplen;

	close(pipefd[0]);
	write(STDIN, MSG_HERE_DOC, ft_strlen(MSG_HERE_DOC));
	line = get_next_line(STDIN);
	while (line)
	{
		cmplen = ft_max(ft_strlen(limiter), ft_strlen(line) - 1);
		if (ft_strncmp(limiter, line, cmplen) == 0)
			break ;
		write(pipefd[1], line, ft_strlen(line));
		free(line);
		write(STDIN, MSG_HERE_DOC, ft_strlen(MSG_HERE_DOC));
		line = get_next_line(STDIN);
	}
	free(line);
	close(pipefd[1]);
	exit(0);
}
