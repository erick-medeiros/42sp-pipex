/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre-f <eandre-f@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/09 15:08:22 by eandre-f          #+#    #+#             */
/*   Updated: 2022/08/13 22:00:44 by eandre-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

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
	char	*runpath;

	while (*paths)
	{
		runpath = ft_strjoin(*paths, "/");
		ft_strupd(&runpath, ft_strjoin(runpath, cmd_exec));
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
	pipex->cmd = NULL;
	while (*envp != NULL && ft_strncmp("PATH", *envp, 4))
		envp++;
	if (envp != NULL)
		pipex->paths = ft_split(*envp + 5, ':');
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
	int	i;

	if (pipe(pipex->pipefd1) < 0 || pipe(pipex->pipefd2) < 0)
		free_error_exit(pipex, 1, ERR_PIPE, NULL);
	i = -1;
	pipex->pipe_number = 1;
	while (++i < pipex->cmd_number)
	{
		pipex->cmd[i]->pid = fork();
		if (pipex->cmd[i]->pid == 0)
		{
			define_stds(pipex, i);
			child_process(pipex, pipex->cmd[i]);
		}
		pipex->pipe_number *= -1;
	}
	close_pipes(pipex);
	i = -1;
	while (++i < pipex->cmd_number)
		waitpid(pipex->cmd[i]->pid, &pipex->cmd[i]->status, 0);
	i = -1;
	while (++i < pipex->cmd_number)
		if (macro_wifexited(pipex->cmd[i]->status))
			pipex->cmd[i]->status = macro_wexitstatus(pipex->cmd[i]->status);
	pipex->exit_status = pipex->cmd[pipex->cmd_number - 1]->status;
}
