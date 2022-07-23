/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyle <donghyle@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 13:43:21 by donghyle          #+#    #+#             */
/*   Updated: 2022/07/08 13:43:22 by donghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_isspace(int c)
{
	if (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r')
		return (1);
	if (c == ' ')
		return (1);
	return (0);
}

int	ft_atoi(const char *str)
{
	char	*cursor;
	long	output;
	long	sign;

	output = 0;
	cursor = (char *) str;
	while (*cursor != '\0' && ft_isspace(*cursor))
		cursor++;
	sign = 1;
	if (*cursor == '+' || *cursor == '-')
	{
		if (*cursor == '-')
			sign = -1;
		cursor++;
	}
	while (*cursor != '\0' && ft_isdigit(*cursor))
	{
		output = output * 10 + ((long)(*cursor - '0'));
		cursor++;
	}
	output *= sign;
	return ((int) output);
}
