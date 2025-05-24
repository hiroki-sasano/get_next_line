/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hisasano <hisasano@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 20:43:37 by hisasano          #+#    #+#             */
/*   Updated: 2025/05/24 05:08:23 by hisasano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef GET_NEXT_LINE_H
#define GET_NEXT_LINE_H

#include <stddef.h>
typedef struct s_fddata
{
	int             fd;
	char            *buf;
	struct s_fddata *prev;
	struct s_fddata *next;
}	t_fddata;


char *get_next_line(int fd);
void get_next_line_utils(t_fddata *fd_list);




#endif