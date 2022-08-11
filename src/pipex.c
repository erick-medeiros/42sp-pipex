/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre-f <eandre-f@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/09 15:08:22 by eandre-f          #+#    #+#             */
/*   Updated: 2022/08/11 17:28:56 by eandre-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

//./pipex file1 cmd1 cmd2 file2

int	pipex_open(char *pathname, int mode)
{
	int	fd;

	if (mode == IN_MODE)
	{
		fd = open(pathname, O_RDONLY);
	}
	else
	{
		fd = open(pathname, O_WRONLY | O_CREAT | O_TRUNC,
				S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
	}
	return (fd);
}

char	*get_runpath(char **paths, char *cmd_exec)
{
	char	*tmp;
	char	*runpath;

	while (*paths)
	{
		tmp = ft_strjoin(*paths, "/");
		runpath = ft_strjoin(tmp, cmd_exec);
		free(tmp);
		if (access(runpath, 0) == 0)
			return (runpath);
		free(runpath);
		paths++;
	}
	return (NULL);
}

void	pipex_cmd(t_cmd *cmd, char *argv_cmd, char **envp)
{
	char	**paths;
	size_t	i;

	cmd->envp = envp;
	cmd->args = ft_split(argv_cmd, ' ');
	while (*envp != NULL && ft_strncmp("PATH", *envp, 4))
		envp++;
	if (!envp)
		cmd->runpath = ft_strdup(cmd->args[0]);
	else
	{
		paths = ft_split(*envp + 5, ':');
		cmd->runpath = get_runpath(paths, cmd->args[0]);
		i = -1;
		while (paths[++i] != NULL)
			free(paths[i]);
		free(paths);
	}
}

int	pipex_tubing(t_pipex *pipex)
{
	if (pipe(pipex->pipefd) < 0)
		error_exit(1, ERR_PIPE, NULL);
	pipex->pid1 = fork();
	if (pipex->pid1 == 0)
	{
		pipex->cmd1.stdin = pipex->infile;
		pipex->cmd1.stdout = pipex->pipefd[1];
		child_process(pipex, &pipex->cmd1);
	}
	pipex->pid2 = fork();
	if (pipex->pid2 == 0)
	{
		pipex->cmd2.stdin = pipex->pipefd[0];
		pipex->cmd2.stdout = pipex->outfile;
		child_process(pipex, &pipex->cmd2);
	}
	close_pipes(pipex);
	waitpid(pipex->pid1, &pipex->cmd1.status, 0);
	waitpid(pipex->pid2, &pipex->cmd2.status, 0);
	return (pipex->cmd2.status);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	if (argc != 5)
		error_exit(1, ERR_ARG, NULL);
	else
	{
		pipex.infile = pipex_open(argv[1], IN_MODE);
		if (pipex.infile < 0)
			error_exit(1, ERR_INFILE, NULL);
		pipex.outfile = pipex_open(argv[4], OUT_MODE);
		if (pipex.outfile < 0)
			error_exit(1, ERR_OUTFILE, NULL);
		pipex_cmd(&pipex.cmd1, argv[2], envp);
		if (!pipex.cmd1.runpath)
			error(ERR_CMD, argv[2]);
		pipex_cmd(&pipex.cmd2, argv[3], envp);
		if (!pipex.cmd2.runpath)
			error(ERR_CMD, argv[3]);
		pipex_tubing(&pipex);
		close(pipex.infile);
		close(pipex.outfile);
		free_pipex(&pipex);
		exit(pipex.cmd2.status);
	}
	return (0);
}
