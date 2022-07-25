#include "libftprintf.h"

void	debug_print_conv(void *c)
{
	t_conv	*conv;

	conv = (t_conv *)c;
	printf(">>> debug_print_conv: i %d\n", conv->i_conv);
	printf(">>> debug_print_conv: [%p:%p]", conv->s, conv->e);
	if (conv->s != NULL && conv->e != NULL)
		printf(", [%d:%d], [%c:%c]", *(conv->s), *(conv->e), *(conv->s), *(conv->e));
	printf("\n");
	if (conv->f_left)
		printf(">>> debug_print_conv: f_left on\n");
	if (conv->f_zeropad)
		printf(">>> debug_print_conv: f_zeropad on\n");
	if (conv->f_precision)
		printf(">>> debug_print_conv: f_precision: %d\n", conv->precision);
	if (conv->f_minwidth)
		printf(">>> debug_print_conv: f_minwidth: %d\n", conv->minwidth);
	if (conv->f_altform)
		printf(">>> debug_print_conv: f_altform on\n");
	if (conv->f_blank)
		printf(">>> debug_print_conv: f_blank on\n");
	if (conv->f_sign)
		printf(">>> debug_print_conv: f_sign on\n");
}
