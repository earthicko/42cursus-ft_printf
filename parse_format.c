/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_format.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghyle <donghyle@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 15:34:48 by donghyle          #+#    #+#             */
/*   Updated: 2022/07/25 15:34:49 by donghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"
#include "./libft/libft.h"

static int	add_plain_text(t_list **convs, char *s, char *e)
{
	t_conv	*new_conv;
	t_list	*new_node;

	printf(">>> add_plain_text: adding [%p:%p] [%c:%c]\n", s, e, *s, *e);
	new_conv = create_conv();
	if (!new_conv)
		return (CODE_ERROR_MALLOC);
	new_conv->i_conv = PLAIN;
	new_conv->s = s;
	new_conv->e = e;
	new_node = ft_lstnew(new_conv);
	if (!new_node)
	{
		del_conv(new_conv);
		return (CODE_ERROR_MALLOC);
	}
	ft_lstadd_back(convs, new_node);
	return (CODE_OK);
}

static int	parse_conversion(t_list **convs, char **format)
{
	t_conv	*new_conv;
	t_list	*new_node;
	char	*start;
	char	*temp;

	printf(">>> parse_conversion: parsing from %s\n", *format);
	new_conv = create_conv();
	if (!new_conv)
		return (CODE_ERROR_MALLOC);
	start = *format;
	(*format)++;
	while (is_printf_flag(**format))
	{
		printf(">>> parse_conversion: found flag %c\n", **format);
		if (**format == SYMBOL_ALTFORM)
			new_conv->f_altform = TRUE;
		else if (**format == SYMBOL_BLANK)
			new_conv->f_blank = TRUE;
		else if (**format == SYMBOL_SIGN)
			new_conv->f_sign = TRUE;
		else if (**format == SYMBOL_LEFT)
			new_conv->f_left = TRUE;
		else if (**format == SYMBOL_ZEROPAD)
			new_conv->f_zeropad = TRUE;
		(*format)++;
	}
	printf(">>> parse_conversion: cursor at %c after flag parsing\n", **format);
	temp = *format;
	while (ft_isdigit(**format))
		(*format)++;
	if (temp != *format)
	{
		new_conv->f_minwidth = TRUE;
		new_conv->minwidth = ft_atoi(temp);
		printf(">>> parse_conversion: found min width %d\n", ft_atoi(temp));
	}
	printf(">>> parse_conversion: cursor at %c after min width parsing\n", **format);
	if (**format == '.')
	{
		printf(">>> parse_conversion: encountered '.'\n");
		new_conv->f_precision = TRUE;
		(*format)++;
		temp = *format;
		while (ft_isdigit(**format))
			(*format)++;
		new_conv->precision = 0;
		if (temp != *format)
		{
			new_conv->precision = ft_atoi(temp);
			printf(">>> parse_conversion: found precision %d\n", ft_atoi(temp));
		}
	}
	printf(">>> parse_conversion: cursor at %c after precision parsing\n", **format);
	if (parse_printf_conv(format, new_conv) != CODE_OK)
	{
		printf(">>> parse_conversion: this sentence is not a valid conversion\n");
		del_conv(new_conv);
		return (CODE_ERROR_GENERIC);
	}
	printf(">>> parse_conversion: found conversion %d\n", new_conv->i_conv);
	new_conv->s = start;
	new_conv->e = *format;
	new_node = ft_lstnew(new_conv);
	if (!new_node)
	{
		del_conv(new_conv);
		return (CODE_ERROR_MALLOC);
	}
	ft_lstadd_back(convs, new_node);
	return (CODE_OK);
}

int	parse_format(t_list **convs, const char *format)
{
	int		n_conv;
	char	*cursor;
	char	*temp;

	n_conv = 0;
	cursor = (char *)format;
	while (*cursor != '\0')
	{
		printf(">>> parse_format: encountered %c\n", *cursor);
		if (*cursor == '%')
		{
			if (parse_conversion(convs, &cursor) == CODE_ERROR_MALLOC)
				return (CODE_ERROR_MALLOC);
		}
		else
		{
			temp = cursor;
			while (*cursor != '%' && *cursor != '\0')
				cursor++;
			printf(">>> parse_format: moved cursor to [%d]\n", *cursor);
			if (add_plain_text(convs, temp, cursor) < 0)
				return (CODE_ERROR_MALLOC);
		}
		n_conv++;
	}
	printf(">>> parse_format: num of conversions: %d\n", n_conv);
	ft_lstiter(*convs, debug_print_conv);
	return (n_conv);
}
