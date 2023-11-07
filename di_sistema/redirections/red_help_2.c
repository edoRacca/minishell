/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   red_help_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraccane <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 17:20:33 by eraccane          #+#    #+#             */
/*   Updated: 2023/11/06 18:25:25 by eraccane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*find_filepath(t_env *e)
{
	int	i;

	i = 0;
	while (e->v[i + 1] != NULL)
		i++;
	return (e->v[i]);
}

void	continuing_minor_double(t_env *e, char *buffer, int pipe_fd[2],
		pid_t pid)
{
	if (pid == -1)
	{
		perror("fork");
		return ;
	}
	if (pid == 0)
	{
		close(pipe_fd[1]);
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
		variabletype(e);
		exiting(e, 1);
	}
	else
	{
		close(pipe_fd[0]);
		write(pipe_fd[1], buffer, ft_strlen(buffer));
		close(pipe_fd[1]);
		waitpid(pid, NULL, 0);
	}
}
