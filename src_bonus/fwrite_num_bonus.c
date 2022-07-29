#include <unistd.h>
#include "ft_printf_bonus.h"

int	fwrite_sdec(int fd, t_conv *conv, int num)
{
	int		res;
	char	*buf[4];

	buf[1] = cstr_prefix(conv, num, 10);
	buf[2] = cstr_nbr(conv, num, CHARSET_DEC);
	buf[3] = cstr_padding(conv, ft_strlen(buf[1]) + ft_strlen(buf[2]));
	buf[0] = merge_num_buffers(conv, buf);
	if (!buf[0])
	{
		abort_fwrite(buf);
		return (CODE_ERROR_MALLOC);
	}
	res = write(fd, buf[0], ft_strlen(buf[0]));
	if (res < 0)
		return (CODE_ERROR_IO);
	abort_fwrite(buf);
	return (res);
}

int	fwrite_uint(int fd, t_conv *conv, t_uint num, char *charset)
{
	int		res;
	char	*buf[4];

	buf[1] = cstr_prefix(conv, num, ft_strlen(charset));
	buf[2] = cstr_nbr(conv, num, charset);
	buf[3] = cstr_padding(conv, ft_strlen(buf[1]) + ft_strlen(buf[2]));
	buf[0] = merge_num_buffers(conv, buf);
	if (!buf[0])
	{
		abort_fwrite(buf);
		return (CODE_ERROR_MALLOC);
	}
	res = write(fd, buf[0], ft_strlen(buf[0]));
	if (res < 0)
		return (CODE_ERROR_IO);
	abort_fwrite(buf);
	return (res);
}
