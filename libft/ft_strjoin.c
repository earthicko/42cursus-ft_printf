/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyle <donghyle@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/10 13:54:28 by donghyle          #+#    #+#             */
/*   Updated: 2022/07/10 13:54:28 by donghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len_1;
	size_t	len_2;
	char	*joined;

	len_1 = ft_strlen(s1);
	len_2 = ft_strlen(s2);
	joined = (char *)malloc(sizeof(char) * (len_1 + len_2 + 1));
	if (!joined)
		return (NULL);
	ft_memcpy(joined, s1, len_1);
	ft_memcpy(joined + len_1, s2, len_2);
	joined[len_1 + len_2] = '\0';
	return (joined);
}
