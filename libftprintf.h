/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libftprintf.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyle <donghyle@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 15:34:52 by donghyle          #+#    #+#             */
/*   Updated: 2022/07/25 15:34:53 by donghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFTPRINTF_H
# define LIBFTPRINTF_H
# include "libft.h"
# include "consts.h"
# include <stdio.h>

typedef struct s_conv
{
	int		i_conv;
	char	*s;
	char	*e;
	int		minwidth;
	int		precision;
	int		f_left;
	int		f_zeropad;
	int		f_precision;
	int		f_minwidth;
	int		f_altform;
	int		f_blank;
	int		f_sign;
}	t_conv;

int		ft_printf(const char *format, ...);

int		parse_format(t_list **convs, const char *format);

int		is_printf_flag(char c);
int		parse_printf_conv(char **fmt, t_conv *conv);

int		init_conv(t_conv *conv);
t_conv	*create_conv(void);
t_conv	*copy_conv(t_conv *orig);
void	del_conv(void *conv);

int		fwrite_plain(int fd, t_conv *conv);
int		fwrite_char(int fd, int c);
int		fwrite_str(int fd, t_conv *conv, char *str);
int		fwrite_ptr(int fd, t_conv *conv, void *ptr);
int		fwrite_pcent(int fd);

int		fwrite_num_prefix(int fd, t_conv *conv, t_ll num, t_ll base);
int		fwrite_sdec(int fd, t_conv *conv, t_ll num);
int		fwrite_udec(int fd, t_conv *conv, t_ll num);
int		fwrite_lhex(int fd, t_conv *conv, t_ll num);
int		fwrite_uhex(int fd, t_conv *conv, t_ll num);

int		ft_putnbr_fd_base(int fd, t_ll nbr, char *base);
size_t	ft_numlen_base(t_ll num, t_ll base);

void	debug_print_conv(void *conv);
#endif
