/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre-f <eandre-f@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/09 15:08:22 by eandre-f          #+#    #+#             */
/*   Updated: 2022/08/12 23:19:58 by eandre-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	pipex_open(char *pathname, int mode)
{
	int	fd;
	int	permissions;

	permissions = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
	if (mode == IN_MODE)
		fd = open(pathname, O_RDONLY);
	else if (mode == OUT_MODE)
		fd = open(pathname, O_WRONLY | O_CREAT | O_TRUNC, permissions);
	else
		fd = open(pathname, O_WRONLY | O_CREAT | O_APPEND, permissions);
	return (fd);
}

static char	*get_runpath(char **paths, char *cmd_exec)
{
	char	*tmp;
	char	*runpath;

	while (*paths)
	{
		tmp = ft_strjoin(*paths, "/");
		runpath = ft_strjoin(tmp, cmd_exec);
		free(tmp);
		if (access(runpath, F_OK | X_OK) == 0)
			return (runpath);
		free(runpath);
		paths++;
	}
	if (access(cmd_exec, F_OK | X_OK) == 0)
		return (ft_strdup(cmd_exec));
	return (NULL);
}

void	pipex_init(t_pipex *pipex, int argc, char **argv, char **envp)
{
	pipex->envp = envp;
	pipex->paths = NULL;
	while (*envp != NULL && ft_strncmp("PATH", *envp, 4))
		envp++;
	if (envp != NULL)
		pipex->paths = ft_split(*envp + 5, ':');
	pipex->infile = pipex_open(argv[1], IN_MODE);
	if (pipex->infile < 0)
		error(ERR_INFILE, NULL);
	pipex->outfile = pipex_open(argv[argc - 1], OUT_MODE);
	if (pipex->outfile < 0)
		error(ERR_OUTFILE, NULL);
}

void	pipex_cmd(t_pipex *pipex, t_cmd *cmd, char *argv_cmd)
{
	cmd->args = ft_split_cmd(argv_cmd, ' ');
	if (pipex->paths == NULL)
		cmd->runpath = ft_strdup(cmd->args[0]);
	else
	{
		cmd->runpath = get_runpath(pipex->paths, cmd->args[0]);
		if (!cmd->runpath)
			cmd->status = 127;
	}
}

void	pipex_tubing(t_pipex *pipex)
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
	if (macro_wifexited(pipex->cmd1.status))
		pipex->cmd1.status = macro_wexitstatus(pipex->cmd1.status);
	if (macro_wifexited(pipex->cmd2.status))
		pipex->cmd2.status = macro_wexitstatus(pipex->cmd2.status);
	pipex->exit_status = pipex->cmd2.status;
}
