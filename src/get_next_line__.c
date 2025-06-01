/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hisasano <hisasano@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 13:56:07 by hisasano          #+#    #+#             */
/*   Updated: 2025/05/27 23:22:08 by hisasano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 4096
#endif

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

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

static char	*select_line(char **buf)
{
	char	*new_line;
	char	*nl_p;
	char	*tmp;
	size_t	len;

	nl_p = my_strchr(*buf, '\n');
	if (nl_p)
		len = (size_t)(nl_p - *buf) + 1;
	else
		len = my_strlen(*buf);
	new_line = malloc(sizeof(char) * (len + 1));
	if (!new_line)
		return (NULL);
	my_memcpy(new_line, *buf, len);
	new_line[len] = '\0';
	tmp = my_strdup((*buf) + len);
	free(*buf);
	*buf = tmp;
	return (new_line);
}

char	*get_next_line(int fd)
{
	char		*buf;
	static char	*str;
	char		*tmp;
	ssize_t		br;

	str = NULL;
	buf = malloc((size_t)BUFFER_SIZE + 1);
	if (!buf)
		return (NULL);
	br = 1;
	while ((str == NULL || !my_strchr(str, '\n')) && br > 0)
	{
		br = read(fd, buf, BUFFER_SIZE);
		if (br < 0)
		{
			free(buf);
			return (NULL);
		}
		buf[br] = '\0';
	}
	free(buf);
	return (select_line(&str));
}
