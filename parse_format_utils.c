#include "libftprintf.h"

int	is_printf_flag(char c)
{
	if (c == '#' || c == ' ' || c == '+' || c == '-' || c == '0')
		return (TRUE);
	return (FALSE);
}

int	parse_printf_conv(char **fmt, t_conv *conv)
{
	if (**fmt == SYMBOL_CHAR)
		conv->i_conv = CHAR;
	else if (**fmt == SYMBOL_STR)
		conv->i_conv = STR;
	else if (**fmt == SYMBOL_PTR)
		conv->i_conv = PTR;
	else if (**fmt == SYMBOL_SDEC1 || **fmt == SYMBOL_SDEC2)
		conv->i_conv = SDEC;
	else if (**fmt == SYMBOL_UDEC)
		conv->i_conv = UDEC;
	else if (**fmt == SYMBOL_LHEX)
		conv->i_conv = LHEX;
	else if (**fmt == SYMBOL_UHEX)
		conv->i_conv = UHEX;
	else if (**fmt == SYMBOL_PCENT)
		conv->i_conv = PCENT;
	else
	{
		conv->i_conv = CODE_ERROR_GENERIC;
		return (CODE_ERROR_GENERIC);
	}
	(*fmt)++;
	return (CODE_OK);
}
