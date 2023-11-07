/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   red_help.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraccane <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 11:45:48 by eraccane          #+#    #+#             */
/*   Updated: 2023/11/06 18:23:33 by eraccane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*
** NOTE - type == 1 serve per la redirection >
** type == 2 serve per la redirection >>
*/
void	fork_pid_zero(t_env *e, char *filename, int type)
{
	int	fd;

	if (type == 1)
		fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0666);
	else if (type == 2)
		fd = open(filename, O_WRONLY | O_APPEND | O_CREAT, 0666);
	if (fd < 0)
	{
		perror("open");
		exiting(e, 0);
	}
	dup2(fd, 1);
	close(fd);
	if (check_builtin(e) == 0)
	{
		flag_matrix(e);
		execve(e->s, e->mat_flag, e->env);
		perror("execve");
		exiting(e, 1);
	}
	variabletype(e);
	exiting(e, 0);
}

void	check_red_fork(t_env *e, char *filename, int type)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		e->exit = 1;
		exiting(e, 1);
	}
	else if (pid == 0)
		fork_pid_zero(e, filename, type);
	else
		waitpid(pid, NULL, 0);
	e->exit = 1;
}

int	index_v_arrows(t_env *e, char *s)
{
	int	i;

	i = 0;
	while (e->v[i])
	{
		if (compare(e->v[i], s) == 1)
			return (i);
		i++;
	}
	return (0);
}

int	search_arrows(t_env *e, char *s)
{
	int	i;

	i = 0;
	while (e->v[i] != NULL)
	{
		if (compare(e->v[i], s) == 1)
		{
			if (e->v[i + 1] == NULL && e->v[i] == s)
			{
				perror("Invalid redirection");
				e->exit = 1;
				return (0);
			}
			else
				return (1);
		}
		i++;
	}
	return (0);
}

char	*find_filepath_minor(t_env *e)
{
	int		i;
	char	*str;

	i = 0;
	str = (char *)malloc(sizeof(char) * ft_strlen(e->v[index_v_arrows(e, "<")
				+ 1]) + 1);
	while (e->v[index_v_arrows(e, "<") + 1][i] != 0)
	{
		str[i] = e->v[index_v_arrows(e, "<") + 1][i];
		i++;
	}
	str[i] = '\0';
	return (str);
}
