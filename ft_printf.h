/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyle <donghyle@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 15:34:52 by donghyle          #+#    #+#             */
/*   Updated: 2022/07/25 15:34:53 by donghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include "libft.h"
# include "consts.h"

typedef struct s_conv
{
	int		i_conv;
	char	*s;
	char	*e;
}	t_conv;

int		ft_printf(const char *format, ...);

int		parse_format(t_list **convs, const char *format);

int		parse_printf_conv(char **fmt, t_conv *conv);

int		init_conv(t_conv *conv);
t_conv	*create_conv(void);
t_conv	*copy_conv(t_conv *orig);
void	del_conv(void *conv);

int		fwrite_plain(int fd, t_conv *conv);
int		fwrite_char(int fd, int c);
int		fwrite_str(int fd, char *str);
int		fwrite_pcent(int fd);

int		fwrite_sdec(int fd, int num);
int		fwrite_udec(int fd, t_uint num);
int		fwrite_lhex(int fd, t_uint num);
int		fwrite_uhex(int fd, t_uint num);

int		fwrite_ptr(int fd, void *ptr);

int		ft_putnbr_fd_base_unsigned(int fd, t_ll nbr, char *base);
size_t	ft_numlen_base(t_ll num, t_ll base);

#endif
