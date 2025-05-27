/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hisasano <hisasano@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 20:40:01 by hisasano          #+#    #+#             */
/*   Updated: 2025/05/27 13:54:55 by hisasano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"
#include <stdlib.h>
#include <unistd.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 4096
#endif

static void	lst_add_back(t_fddata **lst, t_fddata *new)
{
	t_fddata	*cur;

	if (!lst || !new)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	cur = *lst;
	while (cur->next)
		cur = cur->next;
	cur->next = new;
	new->prev = cur;
}

static t_fddata	*get_or_make(t_fddata **head, int fd)
{
	t_fddata	*cur;
	t_fddata	*node;

	cur = *head;
	while (cur)
	{
		if (cur->fd == fd)
			return (cur);
		cur = cur->next;
	}
	node = malloc(sizeof(*node));
	if (!node)
		return (NULL);
	node->fd = fd;
	node->buf = my_strdup("");
	node->next = NULL;
	node->prev = NULL;
	lst_add_back(head, node);
	return (node);
}

static void	lst_remove(t_fddata **head, t_fddata *node)
{
	if (!head || !*head || !node)
		return ;
	if (node->prev)
		node->prev->next = node->next;
	if (node->next)
		node->next->prev = node->prev;
	if (*head == node)
		*head = node->next;
	free(node->buf);
	free(node);
}

static char	*select_line(t_fddata *node)
{
	char	*new_line;
	char	*nl_p;
	char	*tmp;
	size_t	len;

	nl_p = my_strchr(node->buf, '\n');
	if (nl_p)
		len = (size_t)(nl_p - node->buf) + 1;
	else
		len = my_strlen(node->buf);
	new_line = malloc(sizeof(char) * (len + 1));
	if (!new_line)
		return (NULL);
	my_memcpy(new_line, node->buf, len);
	new_line[len] = '\0';
	tmp = my_strdup(node->buf + len);
	free(node->buf);
	node->buf = tmp;
	return (new_line);
}

char	*get_next_line(int fd)
{
	static t_fddata	*head;
	t_fddata		*node;
	char			buf[BUFFER_SIZE + 1];
	int				br;
	char			*tmp;

	node = get_or_make(&head, fd);
	br = 1;
	if (!node)
		return (NULL);
	while (!my_strchr(node->buf, '\n') && br > 0)
	{
		br = read(fd, buf, BUFFER_SIZE);
		if (br <= 0)
			break ;
		buf[br] = '\0';
		tmp = node->buf;
		node->buf = my_strjoin(tmp, buf);
		free(tmp);
		if (!node->buf)
			return (NULL);
	}
	if (br == 0 && (node->buf == NULL || !*node->buf))
		return (lst_remove(&head, node), NULL);
	return (select_line(node));
}
