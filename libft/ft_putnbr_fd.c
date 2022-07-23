/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyle <donghyle@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/10 13:53:55 by donghyle          #+#    #+#             */
/*   Updated: 2022/07/10 13:53:57 by donghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

static void	ft_putdigit(int digit, int fd)
{
	char	buffer;

	buffer = digit + '0';
	write(fd, &buffer, 1);
}

static void	ft_putnbr_unsigned_fd(int n, int fd)
{
	int	digit;
	int	divider;

	divider = 1;
	while (!(-10 < n / divider && n / divider < 10))
		divider *= 10;
	while (divider > 0)
	{
		digit = n / divider;
		n = n - (digit * divider);
		divider /= 10;
		if (digit < 0)
			digit = -digit;
		ft_putdigit(digit, fd);
	}
}

void	ft_putnbr_fd(int n, int fd)
{
	if (n < 0)
		write(fd, "-", 1);
	ft_putnbr_unsigned_fd(n, fd);
}
