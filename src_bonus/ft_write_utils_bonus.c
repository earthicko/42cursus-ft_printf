#include <unistd.h>
#include "ft_printf_bonus.h"

size_t	ft_numlen_base(t_ll num, t_ll base)
{
	size_t	len;

	len = 0;
	while (num != 0)
	{
		num /= base;
		len++;
	}
	return (len);
}

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
	if (write(fd, base + digit, sizeof(char)) < 0)
		return (CODE_ERROR_IO);
	return (n_put + 1);
}

int	ft_putnbr_fd_base_unsigned(int fd, t_ll nbr, char *base)
{
	int	n_put;
	int	l_base;

	l_base = ft_strlen(base);
	if (nbr == 0)
	{
		n_put = write(fd, base, sizeof(char));
		if (n_put < 0)
			return (CODE_ERROR_IO);
		return (n_put);
	}
	n_put = putdigit_fd_base_unsigned(fd, nbr, base, l_base);
	if (n_put < 0)
		return (CODE_ERROR_IO);
	return (n_put);
}
