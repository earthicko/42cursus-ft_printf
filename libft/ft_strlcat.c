/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyle <donghyle@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 12:10:15 by donghyle          #+#    #+#             */
/*   Updated: 2022/07/08 12:10:18 by donghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	len_src;
	size_t	len_dst;
	size_t	i_src;
	size_t	i_dst;

	len_src = ft_strlen(src);
	len_dst = ft_strlen(dst);
	i_src = 0;
	i_dst = len_dst;
	if (dstsize > len_dst)
	{
		while (i_dst < dstsize - 1 && src[i_src] != '\0')
		{
			dst[i_dst] = src[i_src];
			i_src++;
			i_dst++;
		}
		dst[i_dst] = '\0';
	}
	if (dstsize < len_dst)
		return (len_src + dstsize);
	else
		return (len_src + len_dst);
}
