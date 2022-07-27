#include <unistd.h>
#include "ft_printf_bonus.h"

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

static int	fwrite_ptr_prefix(int fd, t_conv *conv, void *ptr)
{
	size_t	len_ptr;
	size_t	i;

	len_ptr = ft_ptrlen(ptr);
	if (conv->f_precision && (size_t)conv->precision > len_ptr)
	{
		i = (size_t)conv->precision - len_ptr;
		while (i > 0)
		{
			if (write(fd, CHARSET_LHEX, sizeof(char)) < 0)
				return (CODE_ERROR_IO);
			i--;
		}
		return ((size_t)conv->precision - len_ptr);
	}
	return (0);
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

int	fwrite_ptr(int fd, t_conv *conv, void *ptr)
{
	int		n_put;
	int		res;

	n_put = write(fd, PREFIX_HEX, L_PREFIX_HEX);
	if (n_put < 0)
		return (CODE_ERROR_IO);
	res = fwrite_ptr_prefix(fd, conv, ptr);
	if (res < 0)
		return (CODE_ERROR_IO);
	n_put += res;
	if (ptr == (void *)0 && conv->f_precision)
		return (n_put);
	res = fwrite_ptr_unsigned(fd, ptr);
	if (res < 0)
		return (CODE_ERROR_IO);
	n_put += res;
	return (n_put);
}
