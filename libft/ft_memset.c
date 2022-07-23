/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyle <donghyle@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 12:10:13 by donghyle          #+#    #+#             */
/*   Updated: 2022/07/08 12:10:14 by donghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*cursor;
	unsigned char	content;

	content = (unsigned char) c;
	cursor = (unsigned char *) b;
	while (len > 0)
	{
		*cursor = content;
		cursor++;
		len--;
	}
	return (b);
}
