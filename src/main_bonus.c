/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre-f <eandre-f@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/12 11:24:08 by eandre-f          #+#    #+#             */
/*   Updated: 2022/08/15 17:51:20 by eandre-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

//./pipex file1 cmd1 cmd2 cmd3 ... cmdn file2

int	pipex_here_doc(t_pipex *pipex, char	*limiter)
{
	int	pipefd[2];
	int	pid;

	if (pipe(pipefd) < 0)
		free_error_exit(pipex, 1, ERR_PIPE, NULL);
	pid = fork();
	if (pid < 0)
		free_error_exit(pipex, 1, ERR_PIPE, NULL);
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

static int	check_args(t_pipex *pipex, int argc, char **argv)
{
	if (argc < 5)
		return (1);
	pipex->here_doc = ft_strncmp(argv[1], "here_doc", 8);
	if ((pipex->here_doc == 0 && argc < 6))
		return (1);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	if (check_args(&pipex, argc, argv) == 1)
	{
		error(ERR_ARG, NULL);
		exit(1);
	}
	else
	{
		pipex_init(&pipex, envp);
		pipex_io(&pipex, argc, argv);
		pipex_commands(&pipex, argc, argv);
		pipex.pipefds = ft_calloc(sizeof(int *), pipex.cmd_number);
		pipex.i = -1;
		while (++pipex.i < pipex.cmd_number - 1)
		{
			pipex.pipefds[pipex.i] = malloc(sizeof(int) * 2);
			if (pipe(pipex.pipefds[pipex.i]) < 0)
				free_error_exit(&pipex, 1, ERR_PIPE, NULL);
		}
		pipex_tubing(&pipex);
		free_pipex(&pipex);
		exit(pipex.exit_status);
	}
	return (0);
}

void	pipex_io(t_pipex *pipex, int argc, char **argv)
{
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
}
