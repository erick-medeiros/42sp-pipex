/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre-f <eandre-f@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/12 11:24:08 by eandre-f          #+#    #+#             */
/*   Updated: 2022/08/13 18:28:13 by eandre-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

//./pipex file1 cmd1 cmd2 cmd3 ... cmdn file2

static void	config_commands(t_pipex *pipex, int argc, char **argv)
{
	int		i;

	pipex->cmd = malloc(sizeof(t_cmd *) * ((argc - 3) + 1));
	if (pipex->cmd == NULL)
		free_error_exit(pipex, 1, ERR_MEM, NULL);
	i = 0;
	while (i < (argc - 3))
	{
		pipex->cmd[i] = malloc(sizeof(t_cmd));
		pipex_cmd(pipex, pipex->cmd[i], argv[i + 2]);
		if (!pipex->cmd[i])
			error(ERR_CMD, argv[i + 2]);
		++i;
	}
	pipex->cmd[i] = NULL;
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	if (argc < 5)
	{
		error(ERR_ARG, NULL);
		exit(1);
	}
	else
	{
		pipex_init(&pipex, argc, argv, envp);
		config_commands(&pipex, argc, argv);
		pipex.cmd_number = argc - 3;
		pipex_tubing(&pipex);
		free_pipex(&pipex);
		exit(pipex.exit_status);
	}
	return (0);
}
