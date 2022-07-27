#include "ft_printf.h"
#include "libft.h"

static int	add_plain_text(t_list **convs, char *s, char *e)
{
	t_conv	*new_conv;
	t_list	*new_node;

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

static int	add_conversion(t_list **convs, t_conv *conv, char *s, char **fmt)
{
	t_list	*new_node;

	conv->s = s;
	conv->e = *fmt;
	new_node = ft_lstnew(conv);
	if (!new_node)
	{
		del_conv(conv);
		return (CODE_ERROR_MALLOC);
	}
	ft_lstadd_back(convs, new_node);
	return (CODE_OK);
}

static int	parse_conversion(t_list **convs, char **format)
{
	t_conv	*new_conv;
	char	*start;

	new_conv = create_conv();
	if (!new_conv)
		return (CODE_ERROR_MALLOC);
	start = *format;
	(*format)++;
	if (parse_printf_conv(format, new_conv) != CODE_OK)
	{
		del_conv(new_conv);
		return (CODE_ERROR_GENERIC);
	}
	return (add_conversion(convs, new_conv, start, format));
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
			if (add_plain_text(convs, temp, cursor) < 0)
				return (CODE_ERROR_MALLOC);
		}
		n_conv++;
	}
	return (n_conv);
}
