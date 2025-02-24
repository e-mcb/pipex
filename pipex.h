/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 22:47:19 by mzutter           #+#    #+#             */
/*   Updated: 2025/02/25 00:58:15 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <stdbool.h>
# include "libft/libft.h"

void	ft_error(const char *message);
char	*ft_pathfinder(char *cmd, char **envp);
void	ft_exec_cmd(char *argv, char **envp);
char **ft_split2(char const *s, char c);
#endif