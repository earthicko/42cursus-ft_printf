/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fwrite_ptr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyle <donghyle@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 16:14:06 by donghyle          #+#    #+#             */
/*   Updated: 2022/07/29 16:14:07 by donghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "ft_printf.h"

static int	ft_ptrlen(void *ptr)
{
	unsigned char	*cursor;
	int				len;

	len = sizeof(ptr) * 2;
	cursor = (unsigned char *)(&ptr) + sizeof(ptr) - 1;
	while (len > 0)
	{
		if (*cursor / 16 != 0)
			break ;
		len--;
		if (*cursor % 16 != 0)
			break ;
		len--;
		cursor--;
	}
	return (len);
}

static int	fwrite_ptr_digits(int fd, int len, unsigned char *c)
{
	int	i;
	int	res;

	i = sizeof(void *) * 2;
	while (i > 0)
	{
		if (i <= len)
		{
			res = write(fd, &CHARSET_LHEX[*c / 16], 1);
			if (res < 0)
				return (res);
		}
		i--;
		if (i <= len)
		{
			res = write(fd, &CHARSET_LHEX[*c % 16], 1);
			if (res < 0)
				return (res);
		}
		i--;
		c--;
	}
	return (CODE_OK);
}

static int	fwrite_ptr_unsigned(int fd, void *ptr)
{
	unsigned char	*cursor;
	int				len;
	int				res;

	if (ptr == NULL)
		return (write(fd, CHARSET_LHEX, 1));
	len = ft_ptrlen(ptr);
	cursor = (unsigned char *)(&ptr) + sizeof(ptr) - 1;
	res = fwrite_ptr_digits(fd, len, cursor);
	if (res < 0)
		return (res);
	return (len);
}

int	fwrite_ptr(int fd, void *ptr)
{
	int		n_put;
	int		res;

	n_put = write(fd, PREFIX_LHEX, L_PREFIX_HEX);
	if (n_put < 0)
		return (n_put);
	res = fwrite_ptr_unsigned(fd, ptr);
	if (res < 0)
		return (res);
	n_put += res;
	return (n_put);
}
