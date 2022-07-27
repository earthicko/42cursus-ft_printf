/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_conv.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyle <donghyle@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 15:34:50 by donghyle          #+#    #+#             */
/*   Updated: 2022/07/25 15:34:51 by donghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "ft_printf.h"

int	init_conv(t_conv *conv)
{
	conv->i_conv = CODE_ERROR_GENERIC;
	conv->s = NULL;
	conv->e = NULL;
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

t_conv	*copy_conv(t_conv *orig)
{
	t_conv	*new_conv;

	new_conv = (t_conv *)malloc(sizeof(t_conv));
	if (!new_conv)
		return (NULL);
	ft_memcpy(new_conv, orig, sizeof(t_conv));
	return (new_conv);
}

void	del_conv(void *conv)
{
	free(conv);
}
