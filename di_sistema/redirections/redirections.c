/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraccane <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 11:12:48 by eraccane          #+#    #+#             */
/*   Updated: 2023/11/06 18:25:58 by eraccane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	multiple_redirect(t_env *e)
{
    (void)e;
}

// << >>
void	redirect_double(t_env *e)
{
	if ((compare(e->v[index_v_arrows(e, ">>")], ">>") == 1
			|| compare(e->v[index_v_arrows(e, ">> ")], ">> ") == 1)
		&& e->v[index_v_arrows(e, ">>") + 1] != NULL)
		double_major_redirect(e);
	else if (compare(e->v[index_v_arrows(e, "<< ")], "<< ") == 1
		&& e->v[index_v_arrows(e, "<<") + 1] != NULL)
		double_minor_redirect(e);
	else
	{
		e->exit = 1;
		errno = EINVAL;
		perror("minishell: warning");
	}
}

// < >
void	redirect_single(t_env *e)
{
	if ((compare(e->v[index_v_arrows(e, ">")], ">") == 1
			|| compare(e->v[index_v_arrows(e, "> ")], "> ") == 1)
		&& e->v[index_v_arrows(e, ">") + 1] != NULL)
		single_major_redirect(e);
	else if ((compare(e->v[index_v_arrows(e, "<")], "<") == 1
			|| compare(e->v[index_v_arrows(e, "< ")], "< ") == 1)
		&& e->v[index_v_arrows(e, "<") + 1] != NULL)
		single_minor_redirect(e);
	else
	{
		e->exit = 1;
		errno = EINVAL;
		perror("minishell: warning");
	}
}

int	check_pid_red(pid_t pid)
{
	if (pid == -1)
	{
		perror("fork");
		return (1);
	}
	else
		return (0);
}
