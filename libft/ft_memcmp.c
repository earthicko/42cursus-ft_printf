/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyle <donghyle@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 13:43:43 by donghyle          #+#    #+#             */
/*   Updated: 2022/07/08 13:43:44 by donghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*cursor_1;
	unsigned char	*cursor_2;

	cursor_1 = (unsigned char *) s1;
	cursor_2 = (unsigned char *) s2;
	while (n > 0)
	{
		if (*cursor_1 != *cursor_2)
			return ((int)(*cursor_1) - (int)(*cursor_2));
		cursor_1++;
		cursor_2++;
		n--;
	}
	return (0);
}
