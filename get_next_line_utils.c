/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hisasano <hisasano@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:31:08 by hisasano          #+#    #+#             */
/*   Updated: 2025/06/06 17:18:48 by hisasano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdint.h>
#include <stdlib.h>

size_t	slen_free(t_fddata *n, const char *s, const char *judge, int mode)
{
	size_t	i;

	i = 0;
	if (mode == F_NODE)
	{
		if (n)
		{
			free(n->buf);
			free(n);
		}
		return (0);
	}
	else if (mode == F_STR && !judge)
	{
		free((char *)s);
		return (0);
	}
	else if (mode == F_NONE)
	{
		while (s[i])
			i++;
		return (i);
	}
	return (1);
}

char	*my_strdup(const char *s)
{
	size_t	len;
	size_t	i;
	char	*dup;

	len = slen_free(NULL, s, NULL, F_NONE);
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

char	*my_strjoin(t_fddata *n, char const *s1, char *s2)
{
	size_t	i;
	size_t	j;
	char	*str;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	if (slen_free(n, s1, s1, 0) > (SIZE_MAX - slen_free(n, s2, s2, 0) - 1))
		return (NULL);
	str = (char *)malloc(slen_free(n, s1, s1, 0) + slen_free(n, s2, s2, 0) + 1);
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
