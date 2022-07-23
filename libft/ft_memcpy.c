/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyle <donghyle@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 12:10:09 by donghyle          #+#    #+#             */
/*   Updated: 2022/07/08 12:10:10 by donghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char	*cursor_src;
	unsigned char	*cursor_dst;

	if (dst == NULL && src == NULL)
		return (dst);
	cursor_src = (unsigned char *) src;
	cursor_dst = (unsigned char *) dst;
	while (n > 0)
	{
		n--;
		*cursor_dst = *cursor_src;
		cursor_src++;
		cursor_dst++;
	}
	return (dst);
}
