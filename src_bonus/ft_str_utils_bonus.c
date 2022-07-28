#include <stdarg.h>
#include "libft.h"
#include "ft_printf_bonus.h"

int	ft_strappend(char **body, const char *tail)
{
	char	*temp;

	if (!tail)
		return (CODE_ERROR_GENERIC);
	temp = ft_strjoin(*body, tail);
	free(*body);
	if (!temp)
		return (CODE_ERROR_MALLOC);
	*body = temp;
	return (CODE_OK);
}

char	*ft_strmerge(int n_str, ...)
{
	char	*buf;
	char	*temp;
	va_list	p_args;
	int		i;

	buf = ft_strdup("");
	if (!buf)
		return (NULL);
	va_start(p_args, n_str);
	i = 0;
	while (i < n_str)
	{
		temp = va_arg(p_args, char *);
		if (ft_strappend(&buf, temp) < 0)
		{
			free(buf);
			return (NULL);
		}
		i++;
	}
	va_end(p_args);
	return (buf);
}
