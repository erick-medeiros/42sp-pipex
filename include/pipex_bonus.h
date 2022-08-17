/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre-f <eandre-f@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 10:45:17 by eandre-f          #+#    #+#             */
/*   Updated: 2022/08/17 16:10:12 by eandre-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# define ERR_HERE_DOC "error in here_doc"
# define MSG_HERE_DOC	"pipex: here_doc> "
# define EXAMPLE_CMD2 "usage: ./pipex file1 cmd1 cmd2 cmd3 ... cmdn file2"
# define EXAMPLE_CMD3 "usage: ./pipex here_doc LIMITER cmd cmd1 file"

int		pipex_here_doc(t_pipex *pipex, char	*limiter);

#endif