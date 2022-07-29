/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fwrite_chr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyle <donghyle@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 16:14:02 by donghyle          #+#    #+#             */
/*   Updated: 2022/07/29 16:14:03 by donghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "ft_printf.h"

int	fwrite_plain(int fd, t_conv *conv)
{
	return (write(fd, conv->s, conv->e - conv->s));
}

int	fwrite_char(int fd, int c)
{
	unsigned char	buf;

	buf = (unsigned char)c;
	return (write(fd, &buf, 1));
}

int	fwrite_str(int fd, char *str)
{
	if (!str)
		str = SYMBOL_NULL;
	return (write(fd, str, ft_strlen(str)));
}
