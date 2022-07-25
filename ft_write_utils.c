#include <unistd.h>
#include "libftprintf.h"

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

static int	putnbr_fd_base_unsigned(int fd, t_ll nbr, char *base, int l_base)
{
	int	digit;

	if (nbr == 0)
		return (CODE_OK);
	digit = nbr % l_base;
	if (digit < 0)
		digit = -digit;
	if (putnbr_fd_base_unsigned(fd, nbr / l_base, base, l_base) < 0)
		return (CODE_ERROR_IO);
	if (write(fd, base + digit, sizeof(char)) < 0)
		return (CODE_ERROR_IO);
	return (CODE_OK);
}

int	ft_putnbr_fd_base(int fd, t_ll nbr, char *base)
{
	int	l_base;

	l_base = ft_strlen(base);
	if (nbr < 0)
	{
		if (write(fd, "-", sizeof(char)) < 0)
			return (CODE_ERROR_IO);
	}
	if (nbr == 0)
	{
		if (write(fd, base, sizeof(char)) < 0)
			return (CODE_ERROR_IO);
		return (CODE_OK);
	}
	return (putnbr_fd_base_unsigned(fd, nbr, base, l_base));
}
