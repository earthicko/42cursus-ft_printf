/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fwrite_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyle <donghyle@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 16:14:08 by donghyle          #+#    #+#             */
/*   Updated: 2022/07/29 16:14:09 by donghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "ft_printf.h"

static int	putdigit_fd_base_unsigned(int fd, t_ll nbr, char *base, int l_base)
{
	int	n_put;
	int	digit;
	int	res;

	if (nbr == 0)
		return (0);
	digit = nbr % l_base;
	if (digit < 0)
		digit = -digit;
	n_put = putdigit_fd_base_unsigned(fd, nbr / l_base, base, l_base);
	if (n_put < 0)
		return (n_put);
	res = write(fd, base + digit, 1);
	if (res < 0)
		return (res);
	return (n_put + res);
}

int	ft_putnbr_fd_base(int fd, t_ll nbr, char *base)
{
	int	n_put;
	int	res;
	int	l_base;

	l_base = ft_strlen(base);
	if (nbr == 0)
		return (write(fd, base, 1));
	n_put = 0;
	if (nbr < 0)
	{
		res = write(fd, PREFIX_NEG, L_PREFIX_NEG);
		if (res < 0)
			return (res);
		n_put += res;
	}
	res = putdigit_fd_base_unsigned(fd, nbr, base, l_base);
	if (res < 0)
		return (res);
	return (n_put + res);
}
