/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre-f <eandre-f@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/09 16:59:51 by eandre-f          #+#    #+#             */
/*   Updated: 2022/08/11 16:57:23 by eandre-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <libft.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>

# define STDIN	0
# define STDOUT	1
# define STDERR	2

# define ERR_INFILE "error on infile"
# define ERR_OUTFILE "error on outfile"
# define ERR_ARG "invalid number of arguments"
# define ERR_PIPE "error on pipe"
# define ERR_FORK "error on fork"
# define ERR_CMD "command not found"

enum	e_mode
{
	IN_MODE,
	OUT_MODE
};

typedef struct s_cmd
{
	char	*runpath;
	char	**args;
	char	**envp;
	int		status;
	int		stdin;
	int		stdout;
}	t_cmd;

typedef struct s_pipex
{
	pid_t	pid1;
	pid_t	pid2;
	int		pipefd[2];
	int		infile;
	int		outfile;
	t_cmd	cmd1;
	t_cmd	cmd2;
}	t_pipex;

void	error(char *err, char *desc);
void	error_exit(int status, char *err, char *desc);
void	close_pipes(t_pipex *pipex);
void	free_cmd(t_cmd *cmd);
void	free_pipex(t_pipex *pipex);
void	child_process(t_pipex *pipex, t_cmd *cmd);

#endif