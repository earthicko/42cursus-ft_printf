#ifndef CONSTS_H
# define CONSTS_H
# define CODE_OK 0
# define CODE_ERROR_GENERIC -1
# define CODE_ERROR_MALLOC -2
# define CODE_ERROR_IO -3
# define SYMBOL_CHAR 'c'
# define SYMBOL_STR 's'
# define SYMBOL_PTR 'p'
# define SYMBOL_SDEC1 'd'
# define SYMBOL_SDEC2 'i'
# define SYMBOL_UDEC 'u'
# define SYMBOL_LHEX 'x'
# define SYMBOL_UHEX 'X'
# define SYMBOL_PCENT '%'
# define SYMBOL_NULL "(null)"
# define CHARSET_DEC "0123456789"
# define CHARSET_LHEX "0123456789abcdef"
# define CHARSET_UHEX "0123456789ABCDEF"
# define PREFIX_HEX "0x"
# define PREFIX_NEG "-"
# define L_PREFIX_HEX 2
# define L_PREFIX_NEG 1

typedef long long			t_ll;
typedef unsigned long long	t_ull;
typedef unsigned int		t_uint;
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
#endif
