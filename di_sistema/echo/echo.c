/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgalli <sgalli@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 10:43:05 by sgalli            #+#    #+#             */
/*   Updated: 2023/10/26 13:17:43 by sgalli           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	get_var(t_env *e, int i, int j)
{
	e->j = 0;
	e->indx = 0;
	while (e->env[e->j] != NULL)
	{
		i = mini_get_var(e, i, j);
		if (i == -1)
			return ;
		e->j++;
	}
}

void	get_var_quote(t_env *e, int j, int iter, int i)
{
	while (e->env[++iter] != NULL)
	{
		if (e->v[j][e->word] == e->env[iter][i])
		{
			while (e->v[j][e->word] == e->env[iter][i] \
			&& e->v[j][e->word] != ' ' && e->v[j][e->word] != 34)
			{
				e->word++;
				i++;
			}
			if (e->env[iter][i] == '=' && (e->v[j][e->word] == ' ' \
			|| e->v[j][e->word] == 34 || e->v[j][e->word] == '$'))
			{
				i++;
				print_str(e->env[iter], i);
				return ;
			}
			else
			{
				e->word = e->numb;
				i = 0;
			}
		}
	}
	update_eword(e, j);
}

void	check_quote(t_env *e, int j)
{
	e->word = 1;
	if (check_closure(e->v[j]) == 1)
		print_str(e->v[j], 0);
	else if (e->v[j][0] == 39)
	{
		while (e->v[j][e->word] != 39)
			printf("%c", e->v[j][e->word++]);
	}
	else
	{
		while (e->v[j][e->word] != 34)
		{
			if (e->v[j][e->word] == '$' && e->v[j][e->word + 1] != '\0' \
		&& e->v[j][e->word + 1] != ' ' && e->v[j][e->word + 1] != 34)
			{
				e->word++;
				e->numb = e->word;
				get_var_quote(e, j, -1, 0);
			}
			else
				printf("%c", e->v[j][e->word++]);
		}
	}
}

void	mini_while(t_env *e)
{
	int	i;

	i = e->i;
	while (e->v[i] != 0 && e->v[i][0] != '|' && e->v[i][0] != '<' && \
	e->v[i][0] != '>')
	{
		if (compare(e->v[i], "$?") == 1)
			printf("%d", e->exit_code);
		if (e->v[i][0] == '$' && e->v[i][1] != '\0')
			get_var(e, 1, i);
		else if (e->v[i][0] == 34 || e->v[i][0] == 39)
			check_quote(e, i);
		else
			writer(e, i);
		i++;
	}
	e->exit_code = 0;
}

void	writeme(t_env *e)
{
	if ((compare(e->v[e->i], "echo") == 1 || compare(e->v[e->i], "echo ") == 1) \
	&& (e->v[e->i + 1] != NULL))
	{
		if (compare(e->v[e->i + 1], "-n") == 1)
		{
			e->i++;
			mini_while(e);
		}
		else
		{
			e->i++;
			mini_while(e);
			printf("\n");
		}
		return ;
	}
	else if (compare(e->v[e->i], "echo") == 1 && e->v[e->i + 1] == NULL)
	{
		e->i++;
		printf("\n");
	}
	else
		directory(e);
}
