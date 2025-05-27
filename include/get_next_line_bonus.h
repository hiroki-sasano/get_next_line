/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hisasano <hisasano@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 20:43:37 by hisasano          #+#    #+#             */
/*   Updated: 2025/05/27 13:58:14 by hisasano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <stddef.h>

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
char				*my_strjoin(char const *s1, char *s2);
char				*my_strdup(const char *s);
size_t				my_strlen(const char *s);

#endif