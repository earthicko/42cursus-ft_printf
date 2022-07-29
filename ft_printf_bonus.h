#ifndef FT_PRINTF_BONUS_H
# define FT_PRINTF_BONUS_H
# include "libft.h"
# include "consts_bonus.h"

typedef struct s_conv
{
	int		i_conv;
	char	*s;
	char	*e;
	int		minwidth;
	int		precision;
	int		f_left;
	int		f_zeropad;
	int		f_precision;
	int		f_minwidth;
	int		f_altform;
	int		f_blank;
	int		f_sign;
}	t_conv;
//		ft_printf_bonus.c
int		ft_printf(const char *format, ...);
//		create_str_bonus.c
char	*cstr_nchars(int n, char c);
char	*cstr_nbr(t_conv *conv, t_ll nbr, char *charset);
char	*cstr_prefix(t_conv *conv, t_ll num, t_ll base);
char	*cstr_padding(t_conv *conv, int len_rest);
//		create_str_utils_bonus.c
int		cstr_nbr_fill_str(char *buf, t_ll nbr, char *base, int l_base);
char	*cstr_sign(t_conv *conv, t_ll num);
int		append_hex_prefix(t_conv *conv, t_ll num, t_ll base, char **buf);
int		append_precision(t_conv *conv, t_ll num, t_ll base, char **buf);
//		ft_numlen_bonus.c
int		ft_numlen_base(t_ll num, t_ll base);
//		ft_str_utils_bonus.c
int		ft_strappend(char **body, const char *tail);
char	*ft_strmerge(int n_str, ...);
//		fwrite_chr_bonus.c
int		fwrite_plain(int fd, t_conv *conv);
int		fwrite_char(int fd, t_conv *conv, int c);
int		fwrite_str(int fd, t_conv *conv, char *str);
int		fwrite_pcent(int fd);
//		fwrite_num_bonus.c
int		fwrite_sdec(int fd, t_conv *conv, int num);
int		fwrite_uint(int fd, t_conv *conv, t_uint num, char *charset);
//		fwrite_ptr_bonus.c
int		fwrite_ptr(int fd, t_conv *conv, void *ptr);
//		fwrite_utils_bonus.c
char	*merge_num_buffers(t_conv *conv, char **buf);
void	abort_fwrite(char **bufs);
int		add_padding(t_conv *conv, char **buf);
//		parse_format_bonus.c
int		parse_format(t_list **convs, const char *format);
//		parse_format_utils_bonus.c
void	parse_printf_flags(t_conv *conv, char **format);
void	parse_printf_minwidth(t_conv *conv, char **format);
void	parse_printf_precision(t_conv *conv, char **format);
int		parse_printf_conv(t_conv *conv, char **fmt);
//		t_conv_bonus.c
t_conv	*create_conv(void);
void	del_conv(void *conv);
#endif
