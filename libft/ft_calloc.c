/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyle <donghyle@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 13:43:26 by donghyle          #+#    #+#             */
/*   Updated: 2022/07/08 13:43:27 by donghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	will_overflow_when_multiplied(size_t A, size_t B)
{
	size_t	result;

	if (A == 0 || B == 0)
		return (0);
	result = A * B;
	if (A == result / B)
		return (0);
	else
		return (1);
}

void	*ft_calloc(size_t count, size_t size)
{
	void	*output;
	size_t	n_bytes;

	if (will_overflow_when_multiplied(count, size))
		return (NULL);
	n_bytes = count * size;
	output = malloc(n_bytes);
	if (output)
		ft_bzero(output, n_bytes);
	return (output);
}
