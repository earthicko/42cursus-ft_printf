#include <stdarg.h>
#include <unistd.h>
#include "libftprintf.h"

static int	fwrite_conv(int fd, const char *fmt, t_list *convs, va_list p_args)
{
	t_conv	*conv;
	int		n_put;

	while (convs != NULL)
	{
		conv = convs->content;
		if (conv->i_conv == PLAIN)
			n_put += fwrite_plain(fd, fmt, conv);
		else if (conv->i_conv == CHAR)
			n_put += fwrite_char(fd, fmt, conv, va_arg(p_args, int));
		else if (conv->i_conv == STR)
			n_put += fwrite_str(fd, fmt, conv, va_arg(p_args, char *));
		else if (conv->i_conv == PTR)
			n_put += fwrite_ptr(fd, fmt, conv, va_arg(p_args, void *));
		else if (conv->i_conv == SDEC)
			n_put += fwrite_sdec(fd, fmt, conv, va_arg(p_args, t_ll));
		else if (conv->i_conv == UDEC)
			n_put += fwrite_udec(fd, fmt, conv, va_arg(p_args, t_ull));
		else if (conv->i_conv == LHEX)
			n_put += fwrite_lhex(fd, fmt, conv, va_arg(p_args, t_ull));
		else if (conv->i_conv == UHEX)
			n_put += fwrite_uhex(fd, fmt, conv, va_arg(p_args, t_ull));
		else if (conv->i_conv == PCENT)
			n_put += fwrite_pcent(fd);
		convs = convs->next;
	}
	return (CODE_OK);
}

int	ft_printf(const char *format, ...)
{
	va_list	p_args;
	int		n_conv;
	t_list	*convs;
	t_list	*node;
	int		n_put;

	(void) node;
	convs = NULL;
	n_conv = parse_format(&convs, format);
	if (n_conv < 0)
	{
		ft_lstclear(&convs, del_conv);
		return (CODE_ERROR_GENERIC);
	}
	va_start(p_args, format);
	n_put = fwrite_conv(STDOUT_FILENO, format, convs, p_args);
	va_end(p_args);
	ft_lstclear(&convs, del_conv);
	return (n_put);
}
