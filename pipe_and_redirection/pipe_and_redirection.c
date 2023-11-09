/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_and_redirection.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgalli <sgalli@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 16:33:21 by sgalli            #+#    #+#             */
/*   Updated: 2023/11/09 17:17:10 by sgalli           ###   ########.fr       */
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

void	red_pipe(t_env *e)
{
}

void	pipe_and_redirection(t_env *e)
{
	int	p;

	p = is_pipe(e);
	if (p == 1)
		pipe_red(e);
	else if (p == 2)
		red_pipe(e);
	else
		return ;
}
