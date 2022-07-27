#include <unistd.h>
#include "ft_printf_bonus.h"

int	fwrite_sdec(int fd, t_conv *conv, int num)
{
	int	n_put;
	int	res;

	n_put = fwrite_num_prefix(fd, conv, num, 10);
	if (n_put < 0)
		return (CODE_ERROR_IO);
	if (num == 0 && conv->f_precision)
		return (n_put);
	res = ft_putnbr_fd_base_unsigned(fd, num, CHARSET_DEC);
	if (res < 0)
		return (CODE_ERROR_IO);
	n_put += res;
	return (n_put);
}

int	fwrite_udec(int fd, t_conv *conv, t_uint num)
{
	int	n_put;
	int	res;

	conv->f_sign = FALSE;
	conv->f_blank = FALSE;
	n_put = fwrite_num_prefix(fd, conv, num, 10);
	if (n_put < 0)
		return (CODE_ERROR_IO);
	if (num == 0 && conv->f_precision)
		return (n_put);
	res = ft_putnbr_fd_base_unsigned(fd, num, CHARSET_DEC);
	if (res < 0)
		return (CODE_ERROR_IO);
	n_put += res;
	return (n_put);
}

int	fwrite_lhex(int fd, t_conv *conv, t_uint num)
{
	int	n_put;
	int	res;

	conv->f_sign = FALSE;
	conv->f_blank = FALSE;
	n_put = fwrite_num_prefix(fd, conv, num, 16);
	if (n_put < 0)
		return (CODE_ERROR_IO);
	if (num == 0 && conv->f_precision)
		return (CODE_OK);
	res = ft_putnbr_fd_base_unsigned(fd, num, CHARSET_LHEX);
	if (res < 0)
		return (CODE_ERROR_IO);
	n_put += res;
	return (n_put);
}

int	fwrite_uhex(int fd, t_conv *conv, t_uint num)
{
	int	n_put;
	int	res;

	conv->f_sign = FALSE;
	conv->f_blank = FALSE;
	n_put = fwrite_num_prefix(fd, conv, num, 16);
	if (n_put < 0)
		return (CODE_ERROR_IO);
	if (num == 0 && conv->f_precision)
		return (CODE_OK);
	res = ft_putnbr_fd_base_unsigned(fd, num, CHARSET_UHEX);
	if (res < 0)
		return (CODE_ERROR_IO);
	n_put += res;
	return (n_put);
}
