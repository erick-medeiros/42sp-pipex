/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre-f <eandre-f@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 13:38:31 by eandre-f          #+#    #+#             */
/*   Updated: 2022/08/15 18:14:11 by eandre-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	pipex_open(char *pathname, int mode)
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

void	pipex_init(t_pipex *pipex, char **envp)
{
	size_t	i;

	pipex->envp = envp;
	pipex->pipefds = NULL;
	pipex->cmd = NULL;
	pipex->exit_status = 0;
	pipex->paths = NULL;
	while (*envp != NULL && ft_strncmp("PATH", *envp, 4))
		envp++;
	if (envp != NULL)
		pipex->paths = ft_split(*envp + 5, ':');
	if (pipex->paths != NULL)
	{
		i = -1;
		while (pipex->paths[++i] != NULL)
			ft_strupd(&pipex->paths[i], ft_strjoin(pipex->paths[i], "/"));
	}
}

static char	*get_runpath(char **paths, char *cmd_exec)
{
	char	*runpath;

	if (paths == NULL)
		return (ft_strdup(cmd_exec));
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

void	pipex_commands(t_pipex *pipex, int argc, char **argv)
{
	int	i;
	int	start;

	start = 2;
	if (pipex->here_doc == 0)
		start = 3;
	pipex->cmd_number = argc - start - 1;
	pipex->cmd = malloc(sizeof(t_cmd *) * (pipex->cmd_number + 1));
	if (pipex->cmd == NULL)
		free_error_exit(pipex, 1, ERR_MEM, NULL);
	i = -1;
	while (++i < pipex->cmd_number)
	{
		pipex->cmd[i] = malloc(sizeof(t_cmd));
		if (pipex->cmd[i] == NULL)
			free_error_exit(pipex, 1, ERR_MEM, NULL);
		pipex->cmd[i]->args = ft_split_cmd(argv[i + start], ' ');
		pipex->cmd[i]->runpath = get_runpath(pipex->paths,
				pipex->cmd[i]->args[0]);
		if (!pipex->cmd[i]->runpath)
			pipex->cmd[i]->status = 127;
		if (!pipex->cmd[i]->runpath)
			error(ERR_CMD, argv[i + start]);
	}
	pipex->cmd[i] = NULL;
}

void	pipex_tubing(t_pipex *pipex)
{
	int	i;

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
	}
	pipex->exit_status = pipex->cmd[pipex->cmd_number - 1]->status;
}
