/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hisasano <hisasano@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 20:40:01 by hisasano          #+#    #+#             */
/*   Updated: 2025/05/25 00:02:28 by hisasano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdlib.h>
#include <unistd.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 4096
#endif

size_t	my_strlen(const char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}

char	*my_strdup(const char *s)
{
	size_t	len;
	size_t	i;
	char	*dup;

	len = my_strlen(s);
	i = 0;
	dup = malloc(sizeof(char) * (len + 1));
	if (!dup)
		return (NULL);
	while (i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

#include <stdint.h>

char	*my_strjoin(char const *s1, char *s2)
{
	size_t	i;
	size_t	j;
	char	*str;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	if (my_strlen(s1) > (SIZE_MAX - my_strlen(s2) - 1))
		return (NULL);
	str = (char *)malloc(my_strlen(s1) + my_strlen(s2) + 1);
	if (!str)
		return (NULL);
	i = 0;
	while (s1[i] != '\0')
	{
		str[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j] != '\0')
	{
		str[i + j] = s2[j];
		j++;
	}
	str[i + j] = '\0';
	return (str);
}

void	lst_add_back(t_fddata **lst, t_fddata *new)
{
	t_fddata	*cur;

	if (!lst || !new)
		return;
	if (*lst == NULL)
	{
		*lst = new;
		return;
	}
	cur = *lst;
	while (cur->next)
		cur = cur->next;
	cur->next = new;
	new->prev = cur;
}

t_fddata	*get_or_make(t_fddata **head, int fd)
{
    t_fddata *cur;
	t_fddata *node;

	cur = *head;
	while (cur)
	{
		if (cur->fd == fd)
			return cur;
		cur = cur->next;
	}
	node = malloc(sizeof(*node));
	if (!node)
		return NULL;
	node->fd = fd;
    node->buf = my_strdup("");
	node->next = NULL;
	node->prev = NULL;
	lst_add_back(head, node);
    return node;
}

char	*my_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i] != (char)c)
	{
		if (s[i] == '\0')
			return (NULL);
		i++;
	}
	return ((char *)&s[i]);
}

static void lst_remove(t_fddata **head, t_fddata *node)
{
    if (!head || !*head || !node) return;
    if (node->prev) node->prev->next = node->next;
    if (node->next) node->next->prev = node->prev;
    if (*head == node) *head = node->next;
    free(node->buf);
    free(node);
}

void	*my_memcpy(void *dst, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*s;
	unsigned char	*d;

	i = 0;
	d = (unsigned char *)dst;
	s = (unsigned char *)src;
	if (!dst && !src)
		return (NULL);
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dst);
}

char    *select_line(t_fddata *node)
{
    char *new_line;
    char *nl_p;
    char *tmp;
    size_t len;

    nl_p = my_strchr(node->buf, '\n');
    if (nl_p)
        len = (size_t)(nl_p - node->buf) + 1;
    else    
        len = my_strlen(node->buf);
    new_line = malloc(sizeof(char) * (len + 1));
    if (!new_line)
        return NULL;
    my_memcpy(new_line, node->buf, len);
    new_line[len] = '\0';
    
    tmp = my_strdup(node->buf + len);
    free(node->buf);
    node->buf = tmp;
    return new_line;
}

char *get_next_line(int fd)
{
	static t_fddata *head;
	t_fddata *node;
    char    buf[BUFFER_SIZE + 1];
    int b_r;
	char *tmp;

	node = get_or_make(&head, fd);
	b_r = 0;
	if (!node)
		return NULL;
    while (!my_strchr(node->buf, '\n') && (b_r = read(fd, buf, BUFFER_SIZE)) > 0)
    {
        buf[b_r] = '\0';
		tmp = node->buf;
		node->buf = my_strjoin(tmp, buf);
		free(tmp);
		if (!node->buf)
			return NULL;
    }
    if (b_r == 0 && (node->buf == NULL || *node->buf == '\0'))
    {
        lst_remove(&head, node);
        return NULL;
    }
    return select_line(node);
}
