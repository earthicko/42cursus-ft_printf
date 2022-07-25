/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_write_num.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyle <donghyle@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 15:34:41 by donghyle          #+#    #+#             */
/*   Updated: 2022/07/25 15:34:42 by donghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libftprintf.h"

int	fwrite_num_prefix(int fd, t_conv *conv, t_ll num, t_ll base)
{
	size_t	len_num;
	size_t	i;

	if (conv->f_sign)
	{
		if (num > 0 && write(fd, PREFIX_POS, L_PREFIX_POS) < 0)
			return (CODE_ERROR_IO);
	}
	else if (conv->f_blank)
	{
		if (num > 0 && write(fd, " ", 1) < 0)
			return (CODE_ERROR_IO);
	}
	len_num = ft_numlen_base(num, base);
	if (conv->f_precision && (size_t)conv->precision > len_num)
	{
		i = 0;
		while (i < (size_t)conv->precision - len_num)
		{
			if (write(fd, "0", sizeof(char)) < 0)
				return (CODE_ERROR_IO);
			i++;
		}
	}
	return (CODE_OK);
}

int	fwrite_sdec(int fd, t_conv *conv, t_ll num)
{
	if (fwrite_num_prefix(fd, conv, num, 10) < 0)
		return (CODE_ERROR_IO);
	if (ft_putnbr_fd_base(fd, num, CHARSET_DEC) < 0)
		return (CODE_ERROR_IO);
	return (CODE_OK);
}

int	fwrite_udec(int fd, t_conv *conv, t_ll num)
{
	(void) fd;
	(void) conv;
	(void) num;
	return (0);
}

int	fwrite_lhex(int fd, t_conv *conv, t_ll num)
{
	(void) fd;
	(void) conv;
	(void) num;
	return (0);
}

int	fwrite_uhex(int fd, t_conv *conv, t_ll num)
{
	(void) fd;
	(void) conv;
	(void) num;
	return (0);
}
