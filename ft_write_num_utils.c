#include <unistd.h>
#include "libftprintf.h"

static int	fwrite_num_sign(int fd, t_conv *conv, t_ll num)
{
	if (num >= 0)
	{
		if (conv->f_sign)
		{
			if (write(fd, PREFIX_POS, L_PREFIX_POS) < 0)
				return (CODE_ERROR_IO);
			return (L_PREFIX_POS);
		}
		else if (conv->f_blank)
		{
			if (write(fd, PREFIX_BLANK, L_PREFIX_BLANK) < 0)
				return (CODE_ERROR_IO);
			return (L_PREFIX_BLANK);
		}
		return (0);
	}
	else
	{
		if (write(fd, PREFIX_NEG, L_PREFIX_NEG) < 0)
			return (CODE_ERROR_IO);
		return (L_PREFIX_NEG);
	}
}

int	fwrite_num_prefix(int fd, t_conv *conv, t_ll num, t_ll base)
{
	size_t	len_num;
	size_t	i;
	int		n_put;

	n_put = fwrite_num_sign(fd, conv, num);
	if (n_put < 0)
		return (CODE_ERROR_IO);
	len_num = ft_numlen_base(num, base);
	if (conv->f_precision && (size_t)conv->precision > len_num)
	{
		i = 0;
		while (i < (size_t)conv->precision - len_num)
		{
			if (write(fd, "0", sizeof(char)) < 0)
				return (CODE_ERROR_IO);
			i++;
		}
		n_put += (int)((size_t)conv->precision - len_num);
	}
	return (n_put);
}
