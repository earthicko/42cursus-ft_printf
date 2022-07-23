#ifndef LIBFTPRINTF_H
# define LIBFTPRINTF_H
# include "libft.h"
# define CODE_OK 0
# define CODE_ERROR_GENERIC -1
# define CODE_ERROR_MALLOC -2

typedef long long t_ll;
typedef unsigned long long t_ull;
enum e_conv
{
	PLAIN = 0,
	CHAR,
	STR,
	PTR,
	SDEC,
	UDEC,
	LHEX,
	UHEX,
	PCENT
};

enum e_bool
{
	FALSE = 0,
	TRUE
};

typedef struct s_conv
{
	int	i_conv;
	int	s;
	int	e;
	int	f_left;
	int	f_zeropad;
	int	f_precision;
	int	f_minwidth;
	int	f_altform;
	int	f_blank;
	int	f_sign;
}		t_conv;

int	ft_printf(const char *format, ...);

int	parse_format(t_list **convs, const char *format);

t_conv	*init_conv(void);
void	del_conv(void *conv);

int	fwrite_plain(int fd, const char *fmt, t_conv *conv);
int	fwrite_char(int fd, const char *fmt, t_conv *conv, int c);
int	fwrite_str(int fd, const char *fmt, t_conv *conv, char *str);
int	fwrite_ptr(int fd, const char *fmt, t_conv *conv, void *ptr);
int	fwrite_pcent(int fd);
int	fwrite_sdec(int fd, const char *fmt, t_conv *conv, t_ll num);
int	fwrite_udec(int fd, const char *fmt, t_conv *conv, t_ull num);
int	fwrite_lhex(int fd, const char *fmt, t_conv *conv, t_ull num);
int	fwrite_uhex(int fd, const char *fmt, t_conv *conv, t_ull num);
#endif
