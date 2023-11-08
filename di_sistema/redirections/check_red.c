/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_red.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgalli <sgalli@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 15:57:03 by sgalli            #+#    #+#             */
/*   Updated: 2023/11/08 19:08:25 by sgalli           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//cat < file1 > file2 > file3

void	read_and_write(int fdin, int fdout, t_env *e)
{
	char	*buf;
	size_t	i;
	int		r;

	r = 1;
	buf = ft_calloc(1024, sizeof(char));
	while (r > 0)
	{
		r = read(fdin, buf, 1024);
		if (r <= 0)
		{
			perror("read");
			break ;
		}
		i = ft_strrlen(buf);
		write(fdout, buf, i);
	}
	close(fdin);
	close(fdout);
	free(buf);
	exiting(e, 1);
}

void	forking_red(t_env *e, char *fileoutput, int type, char *fileinput)
{
	int	fdin;
	int	fdout;

	fdin = open(fileinput, O_RDONLY);
	free(fileinput);
	if (type == 1)
		fdout = open(fileoutput, O_WRONLY | O_TRUNC | O_CREAT, 0666);
	else if (type == 2)
		fdout = open(fileoutput, O_WRONLY | O_APPEND | O_CREAT, 0666);
	free(fileoutput);
	if (fdout < 0 || fdin < 0)
	{
		perror("open");
		e->exit = 1;
		return ;
	}
	dup2(fdin, 0);
	dup2(fdout, 1);
	read_and_write(fdin, fdout, e);
}

void	writing(t_env *e, char *fileoutput, int type, char *fileinput)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		e->exit = 1;
		exiting(e, 1);
	}
	if (pid == 0)
		forking_red(e, fileoutput, type, fileinput);
	else
		waitpid(pid, NULL, 0);
}

void	last_file(t_env *e)
{
	char	*fileinput;
	char	*fileoutput;

	e->i = e->in_red;
	fileinput = find_filepath_minor_mult(e);
	if (compare(e->v[e->in_red], ">>") == 1)
	{
		e->i = e->out_red;
		fileoutput = find_mult_mult_filepath(e);
		writing(e, fileoutput, 2, fileinput);
	}
	else
	{
		e->i = e->out_red;
		fileoutput = find_mult_filepath(e);
		writing(e, fileoutput, 1, fileinput);
	}
}

void	update_in_out(t_env *e)
{
	int	i;

	i = e->i;
	while (e->v[i] != NULL)
	{
		if (compare(e->v[i], "<") == 1)
		{
			e->in_red = i;
			break ;
		}
		else if (compare(e->v[i], ">") == 1)
		{
			e->out_red = i;
			break ;
		}
		i++;
	}
}
