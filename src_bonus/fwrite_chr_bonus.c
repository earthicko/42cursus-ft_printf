#include <unistd.h>
#include "ft_printf_bonus.h"

int	fwrite_plain(int fd, t_conv *conv)
{
	int	n_put;

	n_put = conv->e - conv->s;
	if (write(fd, conv->s, n_put) >= 0)
		return (n_put);
	else
		return (CODE_ERROR_IO);
}

static int	fwrite_char_with_padding(int fd, t_conv *conv, int c, char *pad)
{
	unsigned char	buf;

	buf = c;
	if (!conv->f_left && write(fd, pad, conv->minwidth - 1) < 0)
		return (CODE_ERROR_IO);
	if (write(fd, &buf, 1) < 0)
		return (CODE_ERROR_IO);
	if (conv->f_left && write(fd, pad, conv->minwidth - 1) < 0)
		return (CODE_ERROR_IO);
	return (CODE_OK);
}

int	fwrite_char(int fd, t_conv *conv, int c)
{
	int		n_put;
	int		res;
	char	*padding;

	n_put = 1;
	if (conv->f_minwidth && conv->minwidth > 1)
	{
		padding = cstr_nchars(conv->minwidth - 1, PREFIX_BLANK[0]);
		n_put += conv->minwidth - 1;
	}
	else
		padding = ft_strdup("");
	if (!padding)
		return (CODE_ERROR_MALLOC);
	res = fwrite_char_with_padding(fd, conv, c, padding);
	free(padding);
	if (res < 0)
		return (CODE_ERROR_IO);
	return (n_put);
}

int	fwrite_str(int fd, t_conv *conv, char *str)
{
	int		n_put;
	int		res;
	char	*buf;
	int		len_max;

	len_max = ft_strlen(str);
	if (conv->f_precision && conv->precision < len_max)
		len_max = conv->precision;
	buf = ft_substr(str, 0, len_max);
	if (!buf)
		return (CODE_ERROR_MALLOC);
	if (add_padding(conv, &buf) < 0)
	{
		free(buf);
		return (CODE_ERROR_MALLOC);
	}
	n_put = ft_strlen(buf);
	res = write(fd, buf, n_put);
	free(buf);
	if (res < 0)
		return (CODE_ERROR_IO);
	return (n_put);
}

int	fwrite_pcent(int fd)
{
	char	pcent;

	pcent = SYMBOL_PCENT;
	if (write(fd, &pcent, 1) < 0)
		return (CODE_ERROR_IO);
	else
		return (1);
}
