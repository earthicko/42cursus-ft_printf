# 데이터 형식

- [데이터 형식](implementation_definition.md): `printf` 체계 내에서 사용되는 기호를 정의한다.

# 해석기

- [해석기](implementation_parser.md): `format` 문자열을 해석하여 티켓을 생성한다.

# 출력기

- [출력기](implementation_fwrite.md): 변수를 티켓에 명시된대로 문자열로 변환하여 출력한다.
  - [요소 생성기](implementation_fwrite.md#요소-생성기): 접두사, 여백, 주부의 개념을 정의하고 이를 생성한다.
  - [요소 연결기](implementation_fwrite.md#요소-연결기): 여러 요소를 연결하는 논리를 고안한다.

# 보조 기능

## 문자열 조작

- `libft.a`의 `ft_strjoin()`을 이용하여 주어진 문자열 뒤에 새 문자열을 연장하는 함수를 구현하면 문자열을 순차적으로 연장하는 작업에 유용하다.

```c
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
```

- 복수의 문자열을 하나로 연결하는 함수는 가변 인자를 통해 구현한다.

```c
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
			va_end(p_args);
			free(buf);
			return (NULL);
		}
		i++;
	}
	va_end(p_args);
	return (buf);
}
```

## 티켓, 리스트

- 티켓 구조체를 생성할 시 그 구성 변수를 적절한 값으로 초기화한다.

```c
static int	init_conv(t_conv *conv)
{
	conv->i_conv = CODE_ERROR_GENERIC;
	conv->s = NULL;
	conv->e = NULL;
	conv->minwidth = CODE_ERROR_GENERIC;
	conv->precision = CODE_ERROR_GENERIC;
	conv->f_left = FALSE;
	conv->f_zeropad = FALSE;
	conv->f_precision = FALSE;
	conv->f_minwidth = FALSE;
	conv->f_altform = FALSE;
	conv->f_blank = FALSE;
	conv->f_sign = FALSE;
	return (CODE_OK);
}

t_conv	*create_conv(void)
{
	t_conv	*new_conv;

	new_conv = (t_conv *)malloc(sizeof(t_conv));
	if (!new_conv)
		return (NULL);
	init_conv(new_conv);
	return (new_conv);
}
```

# 주 함수

```c
int	ft_printf(const char *format, ...)
{
	va_list	p_args;
	t_list	*convs;
	int		res;

	convs = NULL;
	res = parse_format(&convs, format);
	if (res < 0)
	{
		ft_lstclear(&convs, del_conv);
		return (res);
	}
	va_start(p_args, format);
	res = fwrite_list(STDOUT_FILENO, convs, p_args);
	va_end(p_args);
	ft_lstclear(&convs, del_conv);
	return (res);
}
```

- **해석기**를 실행하여 티켓 리스트를 생성한다.
  - 실행 중 오류가 발생할 시 음의 정수를 반환한다.
- 티켓 리스트를 **출력기**에 입력한다.
  - 출력기가 음의 정수를 반환할 시 음의 정수를 반환한다.
  - 출력기가 출력한 문자 개수를 반환할 시 이를 반환한다.
