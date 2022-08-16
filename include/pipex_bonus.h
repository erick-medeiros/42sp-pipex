/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre-f <eandre-f@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 10:45:17 by eandre-f          #+#    #+#             */
/*   Updated: 2022/08/16 10:54:37 by eandre-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# define MSG_HERE_DOC	"pipex: here_doc> "

int		check_args(t_pipex *pipex, int argc, char **argv);
int		pipex_here_doc(t_pipex *pipex, char	*limiter);
void	child_here_doc(t_pipex *pipex, int fd[2], char	*limiter);

#endif