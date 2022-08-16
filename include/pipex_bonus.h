/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre-f <eandre-f@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 10:45:17 by eandre-f          #+#    #+#             */
/*   Updated: 2022/08/16 11:16:30 by eandre-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# define MSG_HERE_DOC	"pipex: here_doc> "

int		pipex_here_doc(t_pipex *pipex, char	*limiter);
void	child_here_doc(t_pipex *pipex, int fd[2], char	*limiter);

#endif