#include "libftprintf.h"

int	fwrite_plain(int fd, const char *fmt, t_conv *conv)
{
	(void) fd;
	(void) fmt;
	(void) conv;
	return (0);
}

int	fwrite_char(int fd, const char *fmt, t_conv *conv, int c)
{
	(void) fd;
	(void) fmt;
	(void) conv;
	(void) c;
	return (0);
}

int	fwrite_str(int fd, const char *fmt, t_conv *conv, char *str)
{
	(void) fd;
	(void) fmt;
	(void) conv;
	(void) str;
	return (0);
}

int	fwrite_ptr(int fd, const char *fmt, t_conv *conv, void *ptr)
{
	(void) fd;
	(void) fmt;
	(void) conv;
	(void) ptr;
	return (0);
}

int	fwrite_pcent(int fd)
{
	(void) fd;
	return (0);
}
