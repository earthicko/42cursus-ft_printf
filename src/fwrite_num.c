#include <unistd.h>
#include "ft_printf.h"

int	fwrite_sdec(int fd, int num)
{
	int	n_put;

	n_put = ft_putnbr_fd_base(fd, num, CHARSET_DEC);
	if (n_put < 0)
		return (CODE_ERROR_IO);
	return (n_put);
}

int	fwrite_udec(int fd, t_uint num)
{
	int	n_put;

	n_put = ft_putnbr_fd_base(fd, num, CHARSET_DEC);
	if (n_put < 0)
		return (CODE_ERROR_IO);
	return (n_put);
}

int	fwrite_lhex(int fd, t_uint num)
{
	int	n_put;

	n_put = ft_putnbr_fd_base(fd, num, CHARSET_LHEX);
	if (n_put < 0)
		return (CODE_ERROR_IO);
	return (n_put);
}

int	fwrite_uhex(int fd, t_uint num)
{
	int	n_put;

	n_put = ft_putnbr_fd_base(fd, num, CHARSET_UHEX);
	if (n_put < 0)
		return (CODE_ERROR_IO);
	return (n_put);
}
