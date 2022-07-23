/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyle <donghyle@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 13:43:52 by donghyle          #+#    #+#             */
/*   Updated: 2022/07/08 13:43:53 by donghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	size_t	l_s1;
	char	*dup;

	l_s1 = ft_strlen(s1);
	dup = (char *)malloc(l_s1 + 1);
	if (!dup)
		return (NULL);
	ft_strlcpy(dup, s1, l_s1 + 1);
	return (dup);
}
