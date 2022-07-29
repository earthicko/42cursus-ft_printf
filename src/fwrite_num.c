#include <unistd.h>
#include "ft_printf.h"

int	fwrite_sdec(int fd, int num)
{
	return (ft_putnbr_fd_base(fd, num, CHARSET_DEC));
}

int	fwrite_uint(int fd, t_uint num, char *charset)
{
	return (ft_putnbr_fd_base(fd, num, charset));
}
