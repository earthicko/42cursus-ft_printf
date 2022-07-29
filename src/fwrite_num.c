/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fwrite_num.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyle <donghyle@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 16:14:04 by donghyle          #+#    #+#             */
/*   Updated: 2022/07/29 16:14:05 by donghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "ft_printf.h"

int	fwrite_int(int fd, int num, char *charset)
{
	return (ft_putnbr_fd_base(fd, num, charset));
}

int	fwrite_uint(int fd, t_uint num, char *charset)
{
	return (ft_putnbr_fd_base(fd, num, charset));
}
