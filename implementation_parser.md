# 해석기

```c
int	parse_format(t_list **convs, const char *format)
{
	char	*cursor;
	char	*temp;

	cursor = (char *)format;
	while (*cursor != '\0')
	{
		if (*cursor == '%')
		{
			if (parse_conversion(convs, &cursor) == CODE_ERROR_MALLOC)
				return (CODE_ERROR_MALLOC);
		}
		else
		{
			temp = cursor;
			while (*cursor != '%' && *cursor != '\0')
				cursor++;
			if (add_plain_text(convs, temp, cursor) < 0)
				return (CODE_ERROR_MALLOC);
		}
	}
	return (CODE_OK);
}
```

- `format` 문자열의 각 원소를 가리키는 커서를 생성한다.
- 커서가 `\0`에 도달할 때까지 반복한다:
  - 커서가 현재 `%`에 있다면:
    - **형식 문자열 해석기**를 실행한다.
      - 이때 **형식 문자열 해석기**는 자신이 파싱을 완료한 지점까지 커서를 이동시켜놓는다.
  - 커서가 현재 `%`이 아닌 문자에 있다면:
    - `%`을 가리킬 때 까지 커서를 앞으로 옮긴다.
    - 원래 커서가 있던 자리부터 커서가 멈춘 자리까지 출력할 것을 지시하는 티켓을 리스트 끝에 추가한다.

> `PLAIN` 변환에서 `t_conv.s`와 `t_conv.e`를 지정하는 규칙
> ```
> 문자열 :"Hello, %s!"
> 인덱스 : 0123456789
> ```
> 위 문자열에서 `Hello, ` 부분이 `PLAIN` 변환에 해당된다. 이때 변환할 부분은 `'H'`(0번)에서 `' '`(6번)까지이다.
> 그러므로 티켓에서 `t_conv.s`는 `H`를 가리키도록 (즉 `format + 0`) 설정한다. 하지만 `t_conv.e`는 ` `를 가리키도록 (즉 `format + 6`) 설정하지 않고, 그 다음 문자인 `%`를 가리키도록 (즉 `format + 7`) 한다.
> 이렇게 하면 `e - s` 연산으로 출력할 문자열의 길이를 알아낼 수 있고, `while (s != e) write(fd, s++, 1);` 등의 문장으로 전체를 출력할 수 있는 등 표현이 간결해진다.
> 이외의 변환에서는 `t_conv.s`와 `t_conv.e`가 특별한 의미를 가지지 않는다.

## 형식 문자열 해석기

```c
static int	parse_conversion(t_list **convs, char **format)
{
	t_conv	*new_conv;
	char	*start;

	new_conv = create_conv();
	if (!new_conv)
		return (CODE_ERROR_MALLOC);
	start = *format;
	parse_printf_flags(new_conv, format);
	parse_printf_minwidth(new_conv, format);
	parse_printf_precision(new_conv, format);
	if (parse_printf_conv(new_conv, format) != CODE_OK)
	{
		del_conv(new_conv);
		return (CODE_ERROR_GENERIC);
	}
	return (add_conversion(convs, new_conv, start, format));
}
```

```
%{플래그: #0- +}{최소 너비: 숫자}{정밀도: .과 숫자들}{변환 지정자: [cspdiuxX%]}
```

- 빈 티켓 (`t_conv`)을 하나 생성한다.

```c
static int	is_printf_flag(char c)
{
	if (c == '#' || c == ' ' || c == '+' || c == '-' || c == '0')
		return (TRUE);
	return (FALSE);
}

void	parse_printf_flags(t_conv *conv, char **format)
{
	(*format)++;
	while (is_printf_flag(**format))
	{
		if (**format == SYMBOL_ALTFORM)
			conv->f_altform = TRUE;
		else if (**format == SYMBOL_BLANK)
			conv->f_blank = TRUE;
		else if (**format == SYMBOL_SIGN)
			conv->f_sign = TRUE;
		else if (**format == SYMBOL_LEFT)
			conv->f_left = TRUE;
		else if (**format == SYMBOL_ZEROPAD)
			conv->f_zeropad = TRUE;
		(*format)++;
	}
}
```

- `f_altform`, `f_blank`, `f_sign`, `f_left`, `f_zeropad`: 각 플래그의 유무를 확인하고 이를 티켓에 기록한다.
  - 플래그에 해당하는 문자 `# +-0`가 아닌 문자가 출현할 때까지 커서를 앞으로 옮긴다. 마주친 플래그를 켠다.

```c
void	parse_printf_minwidth(t_conv *conv, char **format)
{
	char	*temp;

	temp = *format;
	while (ft_isdigit(**format))
		(*format)++;
	if (temp != *format)
	{
		conv->f_minwidth = TRUE;
		conv->minwidth = ft_atoi(temp);
	}
}
```

- `f_minwidth`: 최소 너비가 지정되어 있는지 확인하고 지정되었다면 그 값을 해석하여 티켓에 기록한다.
  - 커서가 숫자를 가리키고 있다면 최소 너비가 지정된 것이다.
  - 숫자가 아닌 문자를 마주칠 때까지 커서를 앞으로 옮긴다. 
  - `minwidth`: 숫자가 끝나는 지점까지의 문자열을 `atoi`로 변환한 결과가 최소 너비다. 

```c
void	parse_printf_precision(t_conv *conv, char **format)
{
	char	*temp;

	if (**format == '.')
	{
		conv->f_precision = TRUE;
		(*format)++;
		temp = *format;
		while (ft_isdigit(**format))
			(*format)++;
		conv->precision = 0;
		if (temp != *format)
			conv->precision = ft_atoi(temp);
	}
}
```

- `f_precision`: 정밀도가 지정되어 있는지 확인하고 지정되었다면 그 값을 해석하여 티켓에 기록한다.
  - 커서가 `.`을 가리킨다면 정밀도가 지정된 것이다.
  - 숫자가 아닌 문자를 마주칠 때까지 커서를 앞으로 이동한다.
  - `precision`: 숫자가 발견되지 않았다면 정밀도는 `0`이고, 숫자가 존재할 경우 `atoi`로 정밀도를 획득한다. 

```c
int	parse_printf_conv(t_conv *conv, char **fmt)
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
```

- `i_conv`: 어떤 변환 지정자가 지정되어 있는지 확인하고 이를 티켓에 기록한다. 
- 위 과정 중 등장하면 안되는 문자가 등장하거나 오류가 발생할 시 티켓은 폐기된다.
  - 모든 것이 정상 작동할 시 티켓을 리스트 끝에 추가한다.

## 단순 출력(PLAIN) 티켓 생성기

- 티켓에 `i_conv`와 출력 범위를 지정하고 리스트에 추가한다.

```c
static int	add_plain_text(t_list **convs, char *s, char *e)
{
	t_conv	*new_conv;
	t_list	*new_node;

	new_conv = create_conv();
	if (!new_conv)
		return (CODE_ERROR_MALLOC);
	new_conv->i_conv = PLAIN;
	new_conv->s = s;
	new_conv->e = e;
	new_node = ft_lstnew(new_conv);
	if (!new_node)
	{
		del_conv(new_conv);
		return (CODE_ERROR_MALLOC);
	}
	ft_lstadd_back(convs, new_node);
	return (CODE_OK);
}
```
