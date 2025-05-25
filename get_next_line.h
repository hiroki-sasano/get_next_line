/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hisasano <hisasano@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 20:43:37 by hisasano          #+#    #+#             */
/*   Updated: 2025/05/25 00:02:40 by hisasano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stddef.h>


typedef struct s_fddata
{
	int				fd;
	char			*buf;
	struct s_fddata	*prev;
	struct s_fddata	*next;
}					t_fddata;

char				*get_next_line(int fd);
static char			*select_line(t_fddata *node);
static void			*my_memcpy(void *dst, const void *src, size_t n);
static void			lst_remove(t_fddata **head, t_fddata *node);
static char			*my_strchr(const char *s, int c);
static t_fddata		*get_or_make(t_fddata **head, int fd);
static void			lst_add_back(t_fddata **lst, t_fddata *new);
static char			*my_strjoin(char const *s1, char *s2);
static char			*my_strdup(const char *s);
static size_t		my_strlen(const char *s);

#endif