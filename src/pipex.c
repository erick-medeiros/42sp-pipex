/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre-f <eandre-f@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 13:38:31 by eandre-f          #+#    #+#             */
/*   Updated: 2022/08/16 19:46:17 by eandre-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	pipex_open(char *pathname, int mode)
{
	int	fd;
	int	permissions;

	permissions = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	if (mode == IN_MODE)
		fd = open(pathname, O_RDONLY);
	else if (mode == OUT_MODE)
		fd = open(pathname, O_WRONLY | O_CREAT | O_TRUNC, permissions);
	else if (mode == APPEND_MODE)
		fd = open(pathname, O_WRONLY | O_CREAT | O_APPEND, permissions);
	else
		fd = open(pathname, O_RDWR | O_WRONLY | O_CREAT | O_TRUNC, permissions);
	return (fd);
}

void	pipex_init(t_pipex *pipex, char **envp)
{
	int	i;

	pipex->envp = envp;
	pipex->pipefds = NULL;
	pipex->cmd = NULL;
	pipex->exit_status = 0;
	pipex->paths = NULL;
	while (envp != NULL && *envp != NULL && ft_strncmp("PATH", *envp, 4))
		envp++;
	if (envp != NULL && *envp != NULL)
		pipex->paths = ft_split(*envp + 5, ':');
	i = -1;
	if (pipex->paths != NULL)
		while (pipex->paths[++i] != NULL)
			ft_strupd(&pipex->paths[i], ft_strjoin(pipex->paths[i], "/"));
	pipex->pipefds = ft_calloc(sizeof(int *), pipex->cmd_number);
	if (pipex->pipefds == NULL)
		error_exit(pipex, 1, ERR_MEM);
	i = -1;
	while (++i < pipex->cmd_number - 1)
	{
		pipex->pipefds[i] = malloc(sizeof(int) * 2);
		if (pipex->pipefds[i] == NULL)
			error_exit(pipex, 1, ERR_MEM);
	}
}

static char	*get_runpath(char **paths, char *cmd_exec)
{
	char	*runpath;

	if (paths == NULL || ft_strrchr(cmd_exec, '/') != NULL)
	{
		if (access(cmd_exec, F_OK | X_OK) == 0)
			return (ft_strdup(cmd_exec));
		return (NULL);
	}
	while (*paths)
	{
		runpath = ft_strjoin(*paths, cmd_exec);
		if (access(runpath, F_OK | X_OK) == 0)
			return (runpath);
		free(runpath);
		paths++;
	}
	if (access(cmd_exec, F_OK | X_OK) == 0)
		return (ft_strdup(cmd_exec));
	return (NULL);
}

void	pipex_commands(t_pipex *pipex, char **argv)
{
	int	i;

	pipex->cmd = malloc(sizeof(t_cmd *) * (pipex->cmd_number + 1));
	if (pipex->cmd == NULL)
		error_exit(pipex, 1, ERR_MEM);
	i = -1;
	while (++i < pipex->cmd_number)
	{
		pipex->cmd[i] = malloc(sizeof(t_cmd));
		if (pipex->cmd[i] == NULL)
			error_exit(pipex, 1, ERR_MEM);
		pipex->cmd[i]->desc = argv[pipex->cmd_start + i];
		pipex->cmd[i]->args = ft_split_cmd(pipex->cmd[i]->desc, ' ');
		pipex->cmd[i]->runpath = get_runpath(pipex->paths,
				pipex->cmd[i]->args[0]);
		if (!pipex->cmd[i]->runpath)
			pipex->cmd[i]->status = 127;
	}
	pipex->cmd[i] = NULL;
}

void	pipex_tubing(t_pipex *pipex)
{
	int	i;

	i = -1;
	while (++i < pipex->cmd_number - 1)
		if (pipe(pipex->pipefds[i]) < 0)
			error_exit(pipex, 1, ERR_PIPE);
	i = -1;
	while (++i < pipex->cmd_number)
	{
		pipex->cmd[i]->pid = fork();
		define_stds(pipex, i);
		if (pipex->cmd[i]->pid == 0)
			child_process(pipex, pipex->cmd[i]);
	}
	close_pipes(pipex);
	i = -1;
	while (++i < pipex->cmd_number)
	{
		waitpid(pipex->cmd[i]->pid, &pipex->cmd[i]->status, 0);
		if (WIFEXITED(pipex->cmd[i]->status))
			pipex->cmd[i]->status = WEXITSTATUS(pipex->cmd[i]->status);
		if (pipex->cmd[i]->status != 0)
			child_error(pipex->cmd[i]);
	}
	pipex->exit_status = pipex->cmd[pipex->cmd_number - 1]->status;
}
