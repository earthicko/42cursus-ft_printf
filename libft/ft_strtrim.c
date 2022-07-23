/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyle <donghyle@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/10 13:54:50 by donghyle          #+#    #+#             */
/*   Updated: 2022/07/10 13:54:51 by donghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <limits.h>

static void	ft_build_map(char const *set, int *map)
{
	size_t	i;
	size_t	len;

	i = 0;
	while (i < UCHAR_MAX + 1)
		map[i++] = 0;
	len = ft_strlen(set);
	i = 0;
	while (i < len)
		map[((int)set[i++]) - CHAR_MIN] = 1;
}

static int	ft_is_in_map(char c, int *map)
{
	return (map[(int)c - CHAR_MIN]);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int				map[UCHAR_MAX + 1];
	unsigned int	i_start;
	unsigned int	i_end;
	size_t			len;

	ft_build_map(set, map);
	i_start = 0;
	i_end = ft_strlen(s1);
	len = (size_t) i_end;
	while (i_start < len && ft_is_in_map(s1[i_start], map))
		i_start++;
	while (1 <= i_end && ft_is_in_map(s1[i_end - 1], map))
		i_end--;
	if (i_start < i_end)
		len = i_end - i_start;
	else
		len = 0;
	return (ft_substr(s1, i_start, len));
}
