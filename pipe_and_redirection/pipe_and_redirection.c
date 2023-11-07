/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_and_redirection.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgalli <sgalli@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 16:33:21 by sgalli            #+#    #+#             */
/*   Updated: 2023/11/05 16:58:17 by sgalli           ###   ########.fr       */
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

void	pipe_and_redirection(t_env *e)
{
	int	p;

	p = is_pipe(e);
	if (p == 1)
	{
		printf("pipe");
	}
	else if (p == 2)
	{
		printf(">>");
	}
	else
		return ;
}
