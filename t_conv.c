#include <stdlib.h>
#include "libftprintf.h"

t_conv	*init_conv(void)
{
	t_conv	*new_conv;

	new_conv = (t_conv *)malloc(sizeof(t_conv));
	if (!new_conv)
		return (NULL);
	new_conv->i_conv = CODE_ERROR_GENERIC;
	new_conv->s = CODE_ERROR_GENERIC;
	new_conv->e = CODE_ERROR_GENERIC;
	new_conv->f_left = FALSE;
	new_conv->f_zeropad = FALSE;
	new_conv->f_precision = FALSE;
	new_conv->f_minwidth = FALSE;
	new_conv->f_altform = FALSE;
	new_conv->f_blank = FALSE;
	new_conv->f_sign = FALSE;
	return (new_conv);
}

void	del_conv(void *conv)
{
	free(conv);
}
