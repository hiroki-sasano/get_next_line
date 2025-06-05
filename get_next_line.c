/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hisasano <hisasano@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 20:40:01 by hisasano          #+#    #+#             */
/*   Updated: 2025/06/05 21:40:11 by hisasano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdlib.h>
#include <unistd.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 4096
#endif

static char	*select_line_or_free(t_fddata *node, size_t free_on)
{
	char	*new_line;
	char	*nl_p;
	char	*tmp;
	size_t	len;

	if (free_on)
	{
		free(node->buf);
		free(node);
		return (NULL);
	}
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
		select_line_or_free(new, 1);
		return ;
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

static int	gnl_refill(t_fddata *node, int fd, char *buf)
{
	int		br;
	char	*tmp;

	while (!my_strchr(node->buf, '\n'))
	{
		br = read(fd, buf, BUFFER_SIZE);
		if (br < 0)
			return (-1);
		if (br == 0)
			return (0);
		buf[br] = '\0';
		tmp = node->buf;
		node->buf = my_strjoin(tmp, buf);
		free(tmp);
		if (!node->buf)
			return (-1);
	}
	return (1);
}

char	*get_next_line(int fd)
{
	static t_fddata	*head;
	t_fddata		*node;
	char			*buf;
	int				br;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buf = malloc(BUFFER_SIZE + 1);
	if (!buf)
		return (NULL);
	node = get_or_make(&head, fd);
	if (!node)
	{
		free(buf);
		return (NULL);
	}
	br = gnl_refill(node, fd, buf);
	free(buf);
	if ((br < 0) || (br == 0 && (node->buf == NULL || !*node->buf)))
	{
		remove_or_add(&head, node, 1);
		return (NULL);
	}
	return (select_line_or_free(node, 0));
}
