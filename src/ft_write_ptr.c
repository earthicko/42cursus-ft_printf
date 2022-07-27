#include <unistd.h>
#include "ft_printf.h"

static size_t	ft_ptrlen(void *ptr)
{
	unsigned char	*cursor;
	size_t			len;

	len = sizeof(ptr) * 2;
	cursor = (unsigned char *)(&ptr) + sizeof(ptr) - 1;
	while (len > 0)
	{
		if (*cursor / 16 != 0)
			break ;
		len--;
		if (*cursor % 16 != 0)
			break ;
		len--;
		cursor--;
	}
	return (len);
}

static int	fwrite_ptr_unsigned(int fd, void *ptr)
{
	unsigned char	*cursor;
	size_t			i;
	size_t			len;

	if (ptr == NULL)
	{
		if (write(fd, CHARSET_LHEX, sizeof(char)) < 0)
			return (CODE_ERROR_IO);
		return (1);
	}
	len = ft_ptrlen(ptr);
	cursor = (unsigned char *)(&ptr) + sizeof(ptr) - 1;
	i = sizeof(ptr) * 2;
	while (i > 0)
	{
		if (i <= len && write(fd, &CHARSET_LHEX[*cursor / 16], 1) < 0)
			return (CODE_ERROR_IO);
		i--;
		if (i <= len && write(fd, &CHARSET_LHEX[*cursor % 16], 1) < 0)
			return (CODE_ERROR_IO);
		i--;
		cursor--;
	}
	return (len);
}

int	fwrite_ptr(int fd, void *ptr)
{
	int		n_put;
	int		res;

	n_put = write(fd, PREFIX_HEX, L_PREFIX_HEX);
	if (n_put < 0)
		return (CODE_ERROR_IO);
	res = fwrite_ptr_unsigned(fd, ptr);
	if (res < 0)
		return (CODE_ERROR_IO);
	n_put += res;
	return (n_put);
}
