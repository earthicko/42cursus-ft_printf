/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_conv_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyle <donghyle@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 16:14:41 by donghyle          #+#    #+#             */
/*   Updated: 2022/07/29 16:14:42 by donghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "ft_printf_bonus.h"

static int	init_conv(t_conv *conv)
{
	conv->i_conv = CODE_ERROR_GENERIC;
	conv->s = NULL;
	conv->e = NULL;
	conv->minwidth = CODE_ERROR_GENERIC;
	conv->precision = CODE_ERROR_GENERIC;
	conv->f_left = FALSE;
	conv->f_zeropad = FALSE;
	conv->f_precision = FALSE;
	conv->f_minwidth = FALSE;
	conv->f_altform = FALSE;
	conv->f_blank = FALSE;
	conv->f_sign = FALSE;
	return (CODE_OK);
}

t_conv	*create_conv(void)
{
	t_conv	*new_conv;

	new_conv = (t_conv *)malloc(sizeof(t_conv));
	if (!new_conv)
		return (NULL);
	init_conv(new_conv);
	return (new_conv);
}

void	del_conv(void *conv)
{
	free(conv);
}
