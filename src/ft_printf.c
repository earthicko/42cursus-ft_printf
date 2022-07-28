#include <stdarg.h>
#include <unistd.h>
#include "ft_printf.h"

static int	fwrite_conv_1(int fd, t_conv *cv, va_list p_args)
{
	int	n_put;

	n_put = 0;
	if (cv->i_conv == PLAIN)
		n_put += fwrite_plain(fd, cv);
	else if (cv->i_conv == CHAR)
		n_put += fwrite_char(fd, va_arg(p_args, int));
	else if (cv->i_conv == STR)
		n_put += fwrite_str(fd, va_arg(p_args, char *));
	else if (cv->i_conv == PTR)
		n_put += fwrite_ptr(fd, va_arg(p_args, void *));
	else
		return (CODE_ERROR_GENERIC);
	return (n_put);
}

static int	fwrite_conv_2(int fd, t_conv *cv, va_list p_args)
{
	int	n_put;

	n_put = 0;
	if (cv->i_conv == SDEC)
		n_put += fwrite_sdec(fd, va_arg(p_args, int));
	else if (cv->i_conv == UDEC)
		n_put += fwrite_udec(fd, va_arg(p_args, int));
	else if (cv->i_conv == LHEX)
		n_put += fwrite_lhex(fd, va_arg(p_args, int));
	else if (cv->i_conv == UHEX)
		n_put += fwrite_uhex(fd, va_arg(p_args, int));
	else if (cv->i_conv == PCENT)
		n_put += fwrite_pcent(fd);
	else
		return (CODE_ERROR_GENERIC);
	return (n_put);
}

static int	fwrite_conv(int fd, t_list *convs, va_list p_args)
{
	t_conv	*conv;
	int		n_put;

	n_put = 0;
	while (convs != NULL)
	{
		conv = convs->content;
		if (PLAIN <= conv->i_conv && conv->i_conv < SDEC)
			n_put += fwrite_conv_1(fd, conv, p_args);
		else
			n_put += fwrite_conv_2(fd, conv, p_args);
		convs = convs->next;
	}
	return (n_put);
}

int	ft_printf(const char *format, ...)
{
	va_list	p_args;
	int		res;
	t_list	*convs;
	int		n_put;

	convs = NULL;
	res = parse_format(&convs, format);
	if (res < 0)
	{
		ft_lstclear(&convs, del_conv);
		return (CODE_ERROR_GENERIC);
	}
	va_start(p_args, format);
	n_put = fwrite_conv(STDOUT_FILENO, convs, p_args);
	va_end(p_args);
	ft_lstclear(&convs, del_conv);
	return (n_put);
}
