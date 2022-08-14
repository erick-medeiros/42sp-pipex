/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre-f <eandre-f@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/12 11:24:08 by eandre-f          #+#    #+#             */
/*   Updated: 2022/08/14 00:40:28 by eandre-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

//./pipex file1 cmd1 cmd2 cmd3 ... cmdn file2

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

static void	config_commands(t_pipex *pipex, int argc, char **argv)
{
	int	i;
	int	start;

	if (pipex->here_doc == 0)
		start = 3;
	else
		start = 2;
	pipex->cmd_number = argc - start - 1;
	pipex->cmd = malloc(sizeof(t_cmd *) * (pipex->cmd_number + 1));
	if (pipex->cmd == NULL)
		free_error_exit(pipex, 1, ERR_MEM, NULL);
	i = 0;
	while (i < pipex->cmd_number)
	{
		pipex->cmd[i] = malloc(sizeof(t_cmd));
		pipex_cmd(pipex, pipex->cmd[i], argv[i + start]);
		if (!pipex->cmd[i])
			error(ERR_CMD, argv[i + start]);
		++i;
	}
	pipex->cmd[i] = NULL;
}

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

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	pipex.here_doc = ft_strncmp(argv[1], "here_doc", 8);
	if (argc < 5 || (pipex.here_doc == 0 && argc < 6))
	{
		error(ERR_ARG, NULL);
		exit(1);
	}
	else
	{
		pipex_init(&pipex, argc, argv, envp);
		config_commands(&pipex, argc, argv);
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
