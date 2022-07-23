/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyle <donghyle@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 13:44:02 by donghyle          #+#    #+#             */
/*   Updated: 2022/07/08 13:44:03 by donghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	char	*cursor;
	size_t	l_needle;
	size_t	l_haystack;

	l_needle = ft_strlen(needle);
	l_haystack = ft_strlen(haystack);
	if (len < l_haystack)
		l_haystack = len;
	if (l_haystack < l_needle)
		return (NULL);
	l_haystack = l_haystack - l_needle + 1;
	cursor = (char *) haystack;
	if (l_needle == 0)
		return (cursor);
	while (l_haystack > 0)
	{
		if (ft_strncmp(cursor, needle, l_needle) == 0)
			return (cursor);
		cursor++;
		l_haystack--;
	}
	return (NULL);
}
