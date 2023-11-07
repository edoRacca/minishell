/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   red_help_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgalli <sgalli@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 17:20:33 by eraccane          #+#    #+#             */
/*   Updated: 2023/11/07 19:02:46 by sgalli           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	single_major_mult_redirect(t_env *e)
{
	char	*filename;
	int		fd;

	filename = find_mult_filepath(e);
	if (compare(e->v[e->i], ">") == 1 || compare(e->v[e->i], "> ") == 1)
	{
		fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0666);
		if (fd < 0)
		{
			perror("open");
			exiting(e, 0);
		}
		close(fd);
	}
	else
		check_red_fork(e, filename, 1);
}

void	redirect_mult_single(t_env *e)
{
	if ((compare(e->v[index_v_arrows(e, ">")], ">") == 1
			|| compare(e->v[index_v_arrows(e, "> ")], "> ") == 1)
		&& e->v[index_v_arrows(e, ">") + 1] != NULL)
		single_major_mult_redirect(e);
	else if ((compare(e->v[index_v_arrows(e, "<")], "<") == 1 \
		|| compare(e->v[index_v_arrows(e, "< ")], "< ") == 1)
		&& e->v[index_v_arrows(e, "<") + 1] != NULL)
		single_minor_mult_redirect(e);
	else
	{
		e->exit = 1;
		errno = EINVAL;
		perror("minishell: warning");
	}
}

void	update_redir(t_env *e)
{
	if ((compare(e->v[e->i], ">>") == 1 || compare(e->v[e->i], ">") == 1 || \
		compare(e->v[e->i], "<<") == 1 || compare(e->v[e->i], "<") == 1 || \
		compare(e->v[e->i], ">> ") == 1 || compare(e->v[e->i], "> ") == 1 || \
			compare(e->v[e->i], "<< ") == 1 || compare(e->v[e->i], "< ") == 1)
		&& (e->v[e->i] != NULL))
		e->i++;
	while (e->v[e->i] != NULL)
	{
		printf("%s\n", e->v[e->i]);
		if (compare(e->v[e->i], ">>") == 1 || compare(e->v[e->i], ">") == 1 || \
			compare(e->v[e->i], "<<") == 1 || compare(e->v[e->i], "<") == 1 || \
		compare(e->v[e->i], ">> ") == 1 || compare(e->v[e->i], "> ") == 1 || \
			compare(e->v[e->i], "<< ") == 1 || compare(e->v[e->i], "< ") == 1)
			break ;
		e->i++;
	}
}

char	*find_filepath(t_env *e)
{
	int	i;

	i = 0;
	while (e->v[i + 1] != NULL)
		i++;
	return (e->v[i]);
}

void	continuing_minor_double(t_env *e, char *s, int pipe_fd[2],
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
		write(pipe_fd[1], s, ft_strlen(s));
		close(pipe_fd[1]);
		waitpid(pid, NULL, 0);
	}
	free(s);
}
