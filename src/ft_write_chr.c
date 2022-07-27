#include <unistd.h>
#include "ft_printf.h"

int	fwrite_plain(int fd, t_conv *conv)
{
	int	n_put;

	n_put = sizeof(char) * (conv->e - conv->s);
	if (write(fd, conv->s, n_put) >= 0)
		return (n_put);
	else
		return (CODE_ERROR_IO);
}

int	fwrite_char(int fd, int c)
{
	unsigned char	buf;
	int				n_put;

	buf = (unsigned char)c;
	n_put = sizeof(unsigned char);
	if (write(fd, &buf, n_put) >= 0)
		return (n_put);
	else
		return (CODE_ERROR_IO);
}

int	fwrite_str(int fd, char *str)
{
	int		n_put;

	if (!str)
		str = SYMBOL_NULL;
	n_put = write(fd, str, ft_strlen(str));
	if (n_put < 0)
		return (CODE_ERROR_IO);
	return (n_put);
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
