/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hisasano <hisasano@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 16:17:32 by hisasano          #+#    #+#             */
/*   Updated: 2025/06/10 16:34:47 by hisasano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 4096
#endif

#include <stdlib.h>
#include <unistd.h>

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

static int	gnl_refill(int fd, char **str, char *buf)
{
	int		br;
	char	*tmp;

	br = 1;
	while ((*str == NULL || !my_strchr(*str, '\n')) && br > 0)
	{
		br = read(fd, buf, BUFFER_SIZE);
		if (br < 0)
			return (-1);
		buf[br] = '\0';
		tmp = *str;
		if (tmp)
			*str = my_strjoin(tmp, buf);
		else
			*str = my_strjoin("", buf);
		if (tmp)
			free(tmp);
		if (*str == NULL)
			return (-1);
	}
	return (1);
}

char	*get_next_line(int fd)
{
	char		*buf;
	static char	*str;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buf = malloc((size_t)BUFFER_SIZE + 1);
	if (!buf)
		return (NULL);
	if (!gnl_refill(fd, &str, buf))
	{
		free(buf);
		return (NULL);
	}
	free(buf);
	if (str == NULL || *str == '\0')
	{
		free(str);
		str = NULL;
		return (NULL);
	}
	return (select_line(&str));
}
