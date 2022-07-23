/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyle <donghyle@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/10 14:26:57 by donghyle          #+#    #+#             */
/*   Updated: 2022/07/10 14:26:58 by donghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*cursor;
	t_list	*cursor_next;

	cursor = *lst;
	while (cursor != NULL)
	{
		cursor_next = cursor->next;
		ft_lstdelone(cursor, del);
		cursor = cursor_next;
	}
	*lst = NULL;
}
