#include <unistd.h>
#include "ft_printf.h"

static int	putdigit_fd_base_unsigned(int fd, t_ll nbr, char *base, int l_base)
{
	int	n_put;
	int	digit;

	if (nbr == 0)
		return (0);
	digit = nbr % l_base;
	if (digit < 0)
		digit = -digit;
	n_put = putdigit_fd_base_unsigned(fd, nbr / l_base, base, l_base);
	if (n_put < 0)
		return (CODE_ERROR_IO);
	if (write(fd, base + digit, 1) < 0)
		return (CODE_ERROR_IO);
	return (n_put + 1);
}

int	ft_putnbr_fd_base(int fd, t_ll nbr, char *base)
{
	int	n_put;
	int	res;
	int	l_base;

	l_base = ft_strlen(base);
	if (nbr == 0)
	{
		n_put = write(fd, base, 1);
		if (n_put < 0)
			return (CODE_ERROR_IO);
		return (n_put);
	}
	n_put = 0;
	if (nbr < 0)
	{
		if (write(fd, PREFIX_NEG, L_PREFIX_NEG) < 0)
			return (CODE_ERROR_IO);
		n_put += L_PREFIX_NEG;
	}
	res = putdigit_fd_base_unsigned(fd, nbr, base, l_base);
	if (res < 0)
		return (CODE_ERROR_IO);
	n_put += res;
	return (n_put);
}
