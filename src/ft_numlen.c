/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_numlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyle <donghyle@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 16:14:27 by donghyle          #+#    #+#             */
/*   Updated: 2022/07/29 16:14:28 by donghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_numlen_base(t_ll num, t_ll base)
{
	int	len;

	len = 0;
	while (num != 0)
	{
		num /= base;
		len++;
	}
	return (len);
}
