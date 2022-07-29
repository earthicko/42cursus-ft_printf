#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include "libft.h"
# include "consts.h"

typedef struct s_conv
{
	int		i_conv;
	char	*s;
	char	*e;
}	t_conv;

int		ft_printf(const char *format, ...);

int		parse_format(t_list **convs, const char *format);

int		parse_printf_conv(t_conv *conv, char **fmt);

t_conv	*create_conv(void);
t_conv	*copy_conv(t_conv *orig);
void	del_conv(void *conv);

int		fwrite_plain(int fd, t_conv *conv);
int		fwrite_char(int fd, int c);
int		fwrite_str(int fd, char *str);
int		fwrite_pcent(int fd);

int		fwrite_sdec(int fd, int num);
int		fwrite_uint(int fd, t_uint num, char *charset);

int		fwrite_ptr(int fd, void *ptr);

int		ft_putnbr_fd_base(int fd, t_ll nbr, char *base);

#endif
