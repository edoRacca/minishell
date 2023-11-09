/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_red.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgalli <sgalli@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 15:57:03 by sgalli            #+#    #+#             */
/*   Updated: 2023/11/09 17:03:09 by sgalli           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//cat < file1 > file2 > file3

void	read_and_write(int fdin, int fdout, t_env *e)
{
	e->i = 0;
	e->s = NULL;
	if (check_builtin(e) == 0)
	{
		e->redir = 1;
		pathcmd(e);
		flag_matrix(e);
	}
	if (e->s == NULL)
	{
		variabletype(e);
		close(fdin);
		close(fdout);
		exiting(e, 1);
	}
	close(fdout);
	close(fdin);
	execve(e->s, e->mat_flag, e->env);
	perror("execve");
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
		e->exit_code = 1;
		perror("open");
		return ;
	}
	dup2(fdout, 1);
	dup2(fdin, 0);
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
	e->i = e->out_red;
	if (compare(e->v[e->out_red], ">>") == 1)
	{
		fileoutput = find_lasth_filepath(e);
		writing(e, fileoutput, 2, fileinput);
	}
	else
	{
		fileoutput = find_lasth_filepath(e);
		writing(e, fileoutput, 1, fileinput);
	}
	free(fileinput);
	free(fileoutput);
}

void	update_in_out(t_env *e)
{
	int	i;

	i = e->i;
	while (e->v[i] != NULL)
	{
		if (e->v[i][0] == '<')
		{
			e->in_red = i;
			break ;
		}
		else if (e->v[i][0] == '>')
		{
			e->out_red = i;
			break ;
		}
		i++;
	}
}
