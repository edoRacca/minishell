/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   red_help3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgalli <sgalli@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 15:35:11 by sgalli            #+#    #+#             */
/*   Updated: 2023/11/07 19:02:15 by sgalli           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*find_mult_filepath(t_env *e)
{
	int		i;

	i = e->i;
	while (e->v[i] != NULL)
	{
		if (compare(e->v[i], ">") == 1)
		{
			i++;
			break ;
		}
		i++;
	}
	return (e->v[i]);
}

char	*find_filepath_minor_mult(t_env *e)
{
	int		i;
	char	*str;
	int		j;

	i = e->i;
	j = 0;
	str = (char *)malloc(sizeof(char) * ft_strlen(e->v[index_v_arrows(e, "<")
				+ 1]) + 1);
	while (e->v[i] != NULL)
	{
		if (compare(e->v[i], "<") == 1)
		{
			i++;
			break ;
		}
		i++;
	}
	while (e->v[i][j] != 0 && \
	e->v[i][j + 1] != ' ')
	{
		str[j] = e->v[i][j];
		j++;
	}
	str[i] = '\0';
	return (str);
}

void	single_minor_mult_redirect(t_env *e)
{
	int		fd;
	char	*filename;

	filename = find_filepath_minor_mult(e);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		e->exit = 1;
		return ;
	}
	free(filename);
	single_continuous(e, fd);
	e->exit = 1;
}
