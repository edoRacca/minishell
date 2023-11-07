/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minor_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraccane <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 21:25:41 by eraccane          #+#    #+#             */
/*   Updated: 2023/11/06 18:23:42 by eraccane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	single_continuous(t_env *e, int fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		e->exit = 1;
		exiting(e, 1);
	}
	else if (pid == 0)
	{
		dup2(fd, STDIN_FILENO);
		close(fd);
		variabletype(e);
		exiting(e, 0);
	}
	else
		waitpid(pid, NULL, 0);
}

char	*update_buffer_red(char *line, char *buffer)
{
	char	*all;

	all = (char *)malloc(sizeof(char) * (ft_strlen(buffer) + 1));
	all = ft_strcpy(all, buffer);
	buffer = (char *)malloc(sizeof(char) * (ft_strlen(line) + ft_strlen(all) + 1
				+ 2));
	buffer = ft_strjoin_n(all, line);
	return (buffer);
}

int	check_signals_red(t_env *e, char *line)
{
	if (e->exit_code == 130)
		return (1);
	if (line == NULL)
	{
		errno = EINTR;
		perror("minishell: warning");
		return (1);
	}
	return (0);
}

int	check_error_red(int pipe_fd[2])
{
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (1);
	}
	return (0);
}

// NOTE - can find check_pid_red() in file -> redirections.c
void	redirect_double_arrows(t_env *e, char *buffer)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (check_error_red(pipe_fd) == 1)
		return ;
	pid = fork();
	continuing_minor_double(e, buffer, pipe_fd, pid);
}
