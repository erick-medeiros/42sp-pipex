/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre-f <eandre-f@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 13:38:31 by eandre-f          #+#    #+#             */
/*   Updated: 2022/08/15 14:08:57 by eandre-f         ###   ########.fr       */
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

void	get_paths(t_pipex *pipex, char **envp)
{
	size_t	i;

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
