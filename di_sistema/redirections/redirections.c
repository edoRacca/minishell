/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgalli <sgalli@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 11:12:48 by eraccane          #+#    #+#             */
/*   Updated: 2023/11/07 16:45:31 by sgalli           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	multiple_redirect(t_env *e)
{
	while (e->v[e->i] != NULL)
	{
		if (search_arrows(e, "< ") == 1 || search_arrows(e, "> ") == 1
			|| search_arrows(e, "<") == 1 || search_arrows(e, ">") == 1)
			redirect_mult_single(e);
		// else if (search_arrows(e, "<< ") == 1 || search_arrows(e, ">> ") == 1
		// 	|| search_arrows(e, "<<") == 1 || search_arrows(e, ">>") == 1)
		// 	redirect_mult_double(e);
		update_redir(e);
	}
	e->exit = 1;
}

// << >>
void	redirect_double(t_env *e)
{
	if ((compare(e->v[index_v_arrows(e, ">>")], ">>") == 1
			|| compare(e->v[index_v_arrows(e, ">> ")], ">> ") == 1)
		&& e->v[index_v_arrows(e, ">>") + 1] != NULL)
		double_major_redirect(e);
	else if (compare(e->v[index_v_arrows(e, "<<")], "<<") == 1
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
	else if ((compare(e->v[index_v_arrows(e, "<")], "<") == 1 \
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
