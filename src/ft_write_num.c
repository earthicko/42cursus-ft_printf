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
#include "ft_printf.h"

int	fwrite_sdec(int fd, int num)
{
	int	n_put;

	n_put = ft_putnbr_fd_base_unsigned(fd, num, CHARSET_DEC);
	if (n_put < 0)
		return (CODE_ERROR_IO);
	return (n_put);
}

int	fwrite_udec(int fd, t_uint num)
{
	int	n_put;

	n_put = ft_putnbr_fd_base_unsigned(fd, num, CHARSET_DEC);
	if (n_put < 0)
		return (CODE_ERROR_IO);
	return (n_put);
}

int	fwrite_lhex(int fd, t_uint num)
{
	int	n_put;

	n_put = ft_putnbr_fd_base_unsigned(fd, num, CHARSET_LHEX);
	if (n_put < 0)
		return (CODE_ERROR_IO);
	return (n_put);
}

int	fwrite_uhex(int fd, t_uint num)
{
	int	n_put;

	n_put = ft_putnbr_fd_base_unsigned(fd, num, CHARSET_UHEX);
	if (n_put < 0)
		return (CODE_ERROR_IO);
	return (n_put);
}
