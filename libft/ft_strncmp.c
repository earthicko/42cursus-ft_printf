/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyle <donghyle@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 13:44:00 by donghyle          #+#    #+#             */
/*   Updated: 2022/07/08 13:44:01 by donghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

typedef unsigned char	t_uchar;

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	idx;

	idx = 0;
	while (!(s1[idx] == '\0' && s2[idx] == '\0') && idx < n)
	{
		if (s1[idx] != s2[idx])
			return ((int)((t_uchar)s1[idx] - (t_uchar)s2[idx]));
		idx++;
	}
	return (0);
}
