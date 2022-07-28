#include "ft_printf_bonus.h"

int	ft_numlen_base(t_ll num, t_ll base)
{
	int	len;

	len = 0;
	while (num != 0)
	{
		num /= base;
		len++;
	}
	return (len);
}
