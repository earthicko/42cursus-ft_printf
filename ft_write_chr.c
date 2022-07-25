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
	printf(">>> fwrite_plain: writing %ld bytes from %s\n", conv->e - conv->s, conv->s);
	if (write(fd, conv->s, sizeof(char) * (conv->e - conv->s)) >= 0)
		return (CODE_OK);
	else
		return (CODE_ERROR_IO);
}

int	fwrite_char(int fd, int c)
{
	unsigned char	buf;

	buf = (unsigned char)c;
	if (write(fd, &buf, sizeof(unsigned char)) >= 0)
		return (CODE_OK);
	else
		return (CODE_ERROR_IO);
	return (0);
}

int	fwrite_str(int fd, t_conv *conv, char *str)
{
	size_t	len_max;
	size_t	i;

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
	return (CODE_OK);
}

int	fwrite_ptr(int fd, t_conv *conv, void *ptr)
{
	t_conv	*new_conv;

	new_conv = create_conv();
	if (!new_conv)
		return (CODE_ERROR_MALLOC);
	new_conv->f_precision = conv->precision;
	if (write(fd, PREFIX_HEX, L_PREFIX_HEX) < 0)
	{
		del_conv(new_conv);
		return (CODE_ERROR_IO);
	}
	if (fwrite_num_prefix(fd, conv, (t_ll)ptr, 16) < 0)
	{
		del_conv(new_conv);
		return (CODE_ERROR_IO);
	}
	del_conv(new_conv);
	if (ft_putnbr_fd_base(fd, (t_ll)ptr, CHARSET_LHEX) < 0)
		return (CODE_ERROR_IO);
	return (CODE_OK);
}

int	fwrite_pcent(int fd)
{
	char	pcent;

	pcent = SYMBOL_PCENT;
	write(fd, &pcent, sizeof(char));
	return (1);
}
