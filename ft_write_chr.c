/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_write_chr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyle <donghyle@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 15:34:40 by donghyle          #+#    #+#             */
/*   Updated: 2022/07/25 15:34:40 by donghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libftprintf.h"

int	fwrite_plain(int fd, t_conv *conv)
{
	int	n_put;

	n_put = sizeof(char) * (conv->e - conv->s);
	if (write(fd, conv->s, n_put) >= 0)
		return (n_put);
	else
		return (CODE_ERROR_IO);
}

int	fwrite_char(int fd, int c)
{
	unsigned char	buf;
	int				n_put;

	buf = (unsigned char)c;
	n_put = sizeof(unsigned char);
	if (write(fd, &buf, n_put) >= 0)
		return (n_put);
	else
		return (CODE_ERROR_IO);
}

int	fwrite_str(int fd, t_conv *conv, char *str)
{
	size_t	len_max;
	size_t	i;

	if (!str)
		str = SYMBOL_NULL;
	len_max = ft_strlen(str);
	if (conv->f_precision && (size_t)conv->precision < len_max)
		len_max = conv->precision;
	i = 0;
	while (i < len_max)
	{
		if (write(fd, str + i, sizeof(char)) < 0)
			return (CODE_ERROR_IO);
		i++;
	}
	return (len_max);
}

int	fwrite_pcent(int fd)
{
	char	pcent;

	pcent = SYMBOL_PCENT;
	if (write(fd, &pcent, sizeof(char)) < 0)
		return (CODE_ERROR_IO);
	else
		return (1);
}
