#include <unistd.h>
#include "ft_printf_bonus.h"

int	fwrite_plain(int fd, t_conv *conv)
{
	int	n_put;

	n_put = sizeof(char) * (conv->e - conv->s);
	if (write(fd, conv->s, n_put) >= 0)
		return (n_put);
	else
		return (CODE_ERROR_IO);
}

static int	fwrite_blanks(int fd, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		if (write(fd, " ", sizeof(char)) < 0)
			return (CODE_ERROR_IO);
		i++;
	}
	return (n);
}

int	fwrite_char(int fd, t_conv *conv, int c)
{
	unsigned char	buf;
	int				n_put;
	int				res;

	buf = (unsigned char)c;
	n_put = 0;
	if (!conv->f_left && conv->f_minwidth && conv->minwidth > 1)
	{
		if (fwrite_blanks(fd, conv->minwidth - 1) < 0)
			return (CODE_ERROR_IO);
		n_put += conv->minwidth - 1;
	}
	res = write(fd, &buf, sizeof(unsigned char));
	if (res < 0)
		return (CODE_ERROR_IO);
	n_put += res;
	if (conv->f_left && conv->f_minwidth && conv->minwidth > 1)
	{
		if (fwrite_blanks(fd, conv->minwidth - 1) < 0)
			return (CODE_ERROR_IO);
		n_put += conv->minwidth - 1;
	}
	return (n_put);
}

int	fwrite_str(int fd, t_conv *conv, char *str)
{
	size_t	len_max;
	size_t	i;

	if (!str)
		str = SYMBOL_NULL;
	len_max = ft_strlen(str);
	if (conv->f_precision && (size_t)conv->precision < len_max)
		len_max = conv->precision;
	i = 0;
	while (i < len_max)
	{
		if (write(fd, str + i, sizeof(char)) < 0)
			return (CODE_ERROR_IO);
		i++;
	}
	return (len_max);
}

int	fwrite_pcent(int fd)
{
	char	pcent;

	pcent = SYMBOL_PCENT;
	if (write(fd, &pcent, sizeof(char)) < 0)
		return (CODE_ERROR_IO);
	else
		return (1);
}
