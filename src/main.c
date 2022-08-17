/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre-f <eandre-f@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/12 11:24:08 by eandre-f          #+#    #+#             */
/*   Updated: 2022/08/17 16:08:50 by eandre-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// ./pipex file1 cmd1 cmd2 file2

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	if (argc != 5)
		arg_error(1);
	pipex_io(&pipex, argc, argv);
	pipex_init(&pipex, envp);
	pipex_commands(&pipex, argv);
	pipex_tubing(&pipex);
	free_pipex(&pipex);
	exit(pipex.exit_status);
}

void	arg_error(int status)
{
	write(STDERR, ERR_ARG, ft_strlen(ERR_ARG));
	write(STDERR, "\n", 1);
	write(STDERR, EXAMPLE_CMD1, ft_strlen(EXAMPLE_CMD1));
	write(STDERR, "\n", 1);
	exit(status);
}

void	pipex_io(t_pipex *pipex, int argc, char **argv)
{
	pipex->here_doc = -1;
	pipex->cmd_start = 2;
	pipex->cmd_number = argc - pipex->cmd_start - 1;
	pipex->infile = pipex_open(argv[1], IN_MODE);
	if (pipex->infile < 0)
		perror(argv[1]);
	pipex->outfile = pipex_open(argv[argc - 1], OUT_MODE);
	if (pipex->outfile < 0)
		perror(argv[argc - 1]);
}
