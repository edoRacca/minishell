/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_and_redirection.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgalli <sgalli@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 16:33:21 by sgalli            #+#    #+#             */
/*   Updated: 2023/11/09 18:25:00 by sgalli           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_pipe(t_env *e)
{
	int	i;

	i = 0;
	while (e->v[i] != NULL)
	{
		if (e->v[i][0] == '|')
			return (1);
		else if (e->v[i][0] == '>' || e->v[i][0] == '<')
		{
			return (2);
		}
		i++;
	}
	return (0);
}

//wc -l < /etc/passwd > file1 | wc -l

void	cont_red_pipe2(t_env *e)
{
	char	*file;
	int		i;

	i = e->i;
	e->i = e->in_red;
	file = find_filepath_minor_mult(e);
	(void)file;
	e->i = i;
}

void	cont_red_pipe(t_env *e)
{

	e->pid_pipe = fork();
	if (e->pid_pipe < 0)
	{
		perror("fork");
		exiting(e, 1);
	}
	else if (e->pid_pipe == 0)
		fork_loop(e);
	else
	{
		cont_red_pipe2(e);
		close(e->pipefd[1]);
		close(e->pipefd[0]);
		waitpid(e->pid_pipe, &e->status, 0);
		if (WIFEXITED(e->status))
			e->exit_code = WEXITSTATUS(e->status);
	}
}

void	pipig2(t_env *e)
{
	e->exit_code = 0;
	e->p_i = 0;
	while (e->p_i < e->c_pipe)
	{
		if (pipe(e->pipefd) == -1)
		{
			perror("pipe");
			exiting(e, 1);
		}
		cont_red_pipe(e);
		e->p_i++;
		update_pipe(e);
	}
	e->p_i = 0;
	e->c_pipe = 0;
	dup2(e->stdin, STDIN_FILENO);
	close(e->stdin);
	dup2(e->stdout, STDOUT_FILENO);
	close(e->stdout);
}

void	red_pipe(t_env *e)
{
	e->in_red = 0;
	e->out_red = 0;
	while (e->v[e->i] != NULL)
	{
		while (e->v[e->i][0] != '<' && e->v[e->i][0] != '>')
			e->i++;
		if ((e->v[e->i][1] != '>') && (search_mult_arrows(e, "< ") == 1 \
	|| search_mult_arrows(e, "> ") == 1 || search_mult_arrows(e, "<") == 1 \
	|| search_mult_arrows(e, ">") == 1))
			redirect_mult_single(e);
		else if (search_mult_arrows(e, ">> ") == 1 || \
	search_mult_arrows(e, ">>") == 1)
			redirect_mult_double(e);
		update_redir(e);
		if (e->v[e->i][0] == '|')
		{
			count_pipe(e);
			e->i++;
			pipig2(e);
		}
	}
	e->exit = 1;
}

void	pipe_and_redirection(t_env *e)
{
	int	p;

	p = is_pipe(e);
	if (p == 2)
		red_pipe(e);
	// if (p == 1)
		// pipe_red(e);
	else
		return ;
}
