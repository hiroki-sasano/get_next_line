/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hisasano <hisasano@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 13:57:39 by hisasano          #+#    #+#             */
/*   Updated: 2025/06/06 17:53:54 by hisasano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stddef.h>
# define F_NONE 0
# define F_STR 1
# define F_NODE 2

typedef struct s_fddata
{
	int				fd;
	char			*buf;
	struct s_fddata	*prev;
	struct s_fddata	*next;
}					t_fddata;

char				*get_next_line(int fd);
char				*my_strchr(const char *s, int c);
void				*my_memcpy(void *dst, const void *src, size_t n);
char				*my_strjoin(t_fddata *n, char const *s1, char *s2);
char				*my_strdup(const char *s);
size_t				slen_free(t_fddata *n, const char *s, const char *judge,
						int mode);

#endif
