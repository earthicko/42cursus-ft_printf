/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyle <donghyle@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 12:10:11 by donghyle          #+#    #+#             */
/*   Updated: 2022/07/08 12:10:13 by donghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static void	ft_memmove_rtol(unsigned char *dst, unsigned char *src, size_t len)
{
	src += (len - 1);
	dst += (len - 1);
	while (len > 0)
	{
		(*dst) = (*src);
		dst--;
		src--;
		len--;
	}
}

static void	ft_memmove_ltor(unsigned char *dst, unsigned char *src, size_t len)
{
	while (len > 0)
	{
		(*dst) = (*src);
		dst++;
		src++;
		len--;
	}
}

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char	*cursor_src;
	unsigned char	*cursor_dst;

	if (len == 0)
		return (dst);
	cursor_src = (unsigned char *) src;
	cursor_dst = (unsigned char *) dst;
	if (cursor_src == cursor_dst)
		return (dst);
	if (cursor_src < cursor_dst && cursor_dst < cursor_src + len)
		ft_memmove_rtol(cursor_dst, cursor_src, len);
	else
		ft_memmove_ltor(cursor_dst, cursor_src, len);
	return (dst);
}
