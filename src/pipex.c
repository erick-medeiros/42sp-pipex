/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre-f <eandre-f@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/09 15:08:22 by eandre-f          #+#    #+#             */
/*   Updated: 2022/08/12 11:44:54 by eandre-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
		if (!cmd->runpath)
			cmd->status = 127;
		i = -1;
		while (paths[++i] != NULL)
			free(paths[i]);
		free(paths);
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
