/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre-f <eandre-f@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/09 16:59:51 by eandre-f          #+#    #+#             */
/*   Updated: 2022/08/16 19:01:28 by eandre-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <libft.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>
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
# define ERR_MEM "memory allocation error"

enum	e_mode
{
	IN_MODE,
	OUT_MODE,
	APPEND_MODE
};

typedef struct s_cmd
{
	pid_t	pid;
	char	*desc;
	char	*runpath;
	char	**args;
	int		status;
	int		stdin;
	int		stdout;
}	t_cmd;

typedef struct s_pipex
{
	char	**envp;
	char	**paths;
	int		exit_status;
	int		**pipefds;
	int		pipe_number;
	int		infile;
	int		outfile;
	int		here_doc;
	int		cmd_start;
	int		cmd_number;
	t_cmd	**cmd;
}	t_pipex;

// pipex {
int		pipex_open(char *pathname, int mode);
void	pipex_init(t_pipex *pipex, char **envp);
void	pipex_io(t_pipex *pipex, int argc, char **argv);
void	pipex_commands(t_pipex *pipex, char **argv);
void	pipex_tubing(t_pipex *pipex);
// } pipex

// child_process {
void	define_stds(t_pipex *pipex, int i);
void	child_process(t_pipex *pipex, t_cmd *cmd);
// } child_process

// free {
void	child_error(t_cmd *cmd);
void	error_exit(t_pipex *pipex, int status, char *msg_err);
void	close_pipes(t_pipex *pipex);
void	free_pipex(t_pipex *pipex);
// } free

#endif