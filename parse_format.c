#include "libftprintf.h"
#include "./libft/libft.h"

static int	add_plain_text(t_list **convs, int s, int e)
{
	t_conv	*new_conv;
	t_list	*new_node;

	(void) s;
	(void) e;
	new_conv = init_conv();
	if (!new_conv)
		return (CODE_ERROR_MALLOC);
	new_node = ft_lstnew(new_conv);
	if (!new_node)
	{
		del_conv(new_conv);
		return (CODE_ERROR_MALLOC);
	}
	ft_lstadd_back(convs, new_node);
	return (CODE_OK);
}

static int	parse_conversion(t_list **convs, char **format)
{
	// 오류 시 -1 반환
	// 파싱 성공시
	// 	리스트에 결과 추가
	// 파싱 실패시
	// 	add_plain_text 리스트에 구간 추가
	(void) convs;
	(void) format;
	return (CODE_OK);
}

int	parse_format(t_list **convs, const char *format)
{
	int		n_conv;
	char	*cursor;
	char	*temp;

	n_conv = 0;
	cursor = (char *)format;
	while (*cursor != '\0')
	{
		if (*cursor == '%')
		{
			if (parse_conversion(convs, &cursor) < 0)
				return (CODE_ERROR_MALLOC);
		}
		else
		{
			temp = cursor;
			while (*temp != '%' && *temp != '\0')
				temp++;
			if (add_plain_text(convs, cursor - format, temp - format) < 0)
				return (CODE_ERROR_MALLOC);
			cursor = temp;
		}
		n_conv++;
	}
	return (n_conv);
}
