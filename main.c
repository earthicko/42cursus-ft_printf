/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyle <donghyle@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 16:12:59 by donghyle          #+#    #+#             */
/*   Updated: 2022/07/29 16:13:01 by donghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <stdio.h>
#include <limits.h>

int	main(void)
{
	char	*base;
	char	num;
	int		ret1;
	int		ret2;

	base = ">------------<%2c>------------<";
	num = 0;
	ret1 = ft_printf(base, num);
	printf("\n");
	ret2 = printf(base, num);
	printf("\n");
	printf("ret %d %d\n", ret1, ret2);
	printf("\n");
	return (0);
}
