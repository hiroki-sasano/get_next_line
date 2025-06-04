/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hisasano <hisasano@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 20:40:01 by hisasano          #+#    #+#             */
/*   Updated: 2025/06/04 20:32:21 by hisasano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdlib.h>
#include <unistd.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 4096
#endif

static void	remove_or_add(t_fddata **lst, t_fddata *new, size_t rmv_on)
{
	t_fddata	*cur;

	if (!lst || !new || (rmv_on && !*lst))
		return ;
	if (rmv_on)
	{
		if (new->prev)
			new->prev->next = new->next;
		if (new->next)
			new->next->prev = new->prev;
		if (*lst == new)
			*lst = new->next;
		free(new->buf);
		free(new);
	}
	if (!*lst)
		*lst = new;
	else
	{
		cur = *lst;
		while (cur->next)
			cur = cur->next;
		cur->next = new;
		new->prev = cur;
	}
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
	remove_or_add(head, node, 0);
	return (node);
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

static int	gnl_refill(t_fddata **head, t_fddata *node, int fd, char *buf)
{
	int		br;
	char	*tmp;

	br = 1;
	while (!my_strchr(node->buf, '\n') && br > 0)
	{
		br = read(fd, buf, BUFFER_SIZE);
		if (br <= 0)
		{
			remove_or_add(head, node, 1);
			return (-1);
		}
		buf[br] = '\0';
		tmp = node->buf;
		node->buf = my_strjoin(tmp, buf);
		free(tmp);
		if (!node->buf)
			return (-1);
	}
	return (br);
}

char	*get_next_line(int fd)
{
	static t_fddata	*head;
	t_fddata		*node;
	char			*buf;
	int				br;

	buf = malloc(BUFFER_SIZE + 1);
	if (!buf)
		return (NULL);
	node = get_or_make(&head, fd);
	if (!node)
		return (free(buf), NULL);
	br = gnl_refill(&head, node, fd, buf);
	free(buf);
	if (br == -1)
		return (NULL);
	if (br == 0 && (node->buf == NULL || !*node->buf))
	{
		remove_or_add(&head, node, 1);
		return (NULL);
	}
	return (select_line(node));
}
