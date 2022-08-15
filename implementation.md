# 데이터 형식

- `printf` 체계 내에서 사용되는 기호를 정의한다.

```c
// 플래그
#define SYMBOL_ALTFORM '#'
#define SYMBOL_BLANK ' '
#define SYMBOL_SIGN '+'
#define SYMBOL_LEFT '-'
#define SYMBOL_ZEROPAD '0'
// 변환 지정자
#define SYMBOL_CHAR 'c'
#define SYMBOL_STR 's'
#define SYMBOL_PTR 'p'
#define SYMBOL_SDEC1 'd'
#define SYMBOL_SDEC2 'i'
#define SYMBOL_UDEC 'u'
#define SYMBOL_LHEX 'x'
#define SYMBOL_UHEX 'X'
#define SYMBOL_PCENT '%'
// FreeBSD의 특이한 NULL 처리
#define SYMBOL_NULL "(null)"
// 각 진법별 사용 문자
#define CHARSET_DEC "0123456789"
#define CHARSET_LHEX "0123456789abcdef"
#define CHARSET_UHEX "0123456789ABCDEF"
// 부호 및 접두사
#define PREFIX_LHEX "0x"
#define PREFIX_UHEX "0X"
#define PREFIX_POS "+"
#define PREFIX_NEG "-"
#define PREFIX_BLANK " "
```

- "티켓"은 다음과 같이 표현한다.

```c
typedef struct s_conv
{ 
    int     i_conv;         // 수행할 변환의 번호
    char    *s;             // format에서 해당 변환의 시작점
    char    *e;             // format에서 해당 변환의 시작점
    int     minwidth;       // 최소 너비 값
    int     precision;      // 정밀도 값
    int     f_minwidth;     // 최소 너비 적용 여부
    int     f_left;         // [-] 좌측 정렬 여부
    int     f_zeropad;      // [0] 패딩을 0으로 쓸 것인지 여부
    int     f_precision;    // [.] 정밀도 적용 여부
    int     f_altform;      // [#] 대체 형태 적용 여부
    int     f_blank;        // [ ] 양의 부호 자리에 공백 출력 여부
    int     f_sign;         // [+] 양의 부호 출력 여부
}	t_conv;
```

- 수행할 변환의 번호는 다음과 같은 열거형에서 정의한다.

```c
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
```

- `PLAIN = 0`: `format` 문자열의 특정 부분을 변환 없이 그대로 출력할 것을 지시한다.
- `CHAR = 1`: `c` 변환에 해당한다. (Char)
- `STR = 2`: `s` 변환에 해당한다. (String)
- `PTR = 3`: `p` 변환에 해당한다. (Pointer)
- `SDEC = 4`: `d`, `i` 변환에 해당한다. (Signed Decimal)
- `UDEC = 5`: `u` 변환에 해당한다. (Unsigned Decimal)
- `LHEX = 6`: `x` 변환에 해당한다. (Lowercase Hexadecimal)
- `UHEX = 7`: `X` 변환에 해당한다. (Uppercase Hexadecimal)
- `PCENT = 8`: `%` 변환에 해당한다. (Percent)

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

# 출력기

티켓 연결 리스트(`t_list`)를 처음부터 끝까지 출력하는 **리스트 출력기**가 있고, 티켓 하나만을 출력하는 **티켓 출력기**가 있다. 각 변환에 대한 논리가 모두 다르기 때문에 여러 종류의 티켓 출력기가 필요하다.

## 리스트 출력기

```c
static int	fwrite_conv(int fd, t_conv *cv, va_list p_args)
{
	if (cv->i_conv == PLAIN)
		return (fwrite_plain(fd, cv));
	else if (cv->i_conv == CHAR)
		return (fwrite_char(fd, cv, va_arg(p_args, int)));
	else if (cv->i_conv == STR)
		return (fwrite_str(fd, cv, va_arg(p_args, char *)));
	else if (cv->i_conv == PTR)
		return (fwrite_ptr(fd, cv, va_arg(p_args, void *)));
	else if (cv->i_conv == SDEC)
		return (fwrite_int(fd, cv, va_arg(p_args, int), CHARSET_DEC));
	else if (cv->i_conv == UDEC)
		return (fwrite_uint(fd, cv, va_arg(p_args, int), CHARSET_DEC));
	else if (cv->i_conv == LHEX)
		return (fwrite_uint(fd, cv, va_arg(p_args, int), CHARSET_LHEX));
	else if (cv->i_conv == UHEX)
		return (fwrite_uint(fd, cv, va_arg(p_args, int), CHARSET_UHEX));
	else if (cv->i_conv == PCENT)
		return (fwrite_char(fd, cv, SYMBOL_PCENT));
	else
		return (CODE_ERROR_GENERIC);
}

static int	fwrite_list(int fd, t_list *convs, va_list p_args)
{
	t_conv	*conv;
	int		n_put;
	int		res;

	n_put = 0;
	while (convs != NULL)
	{
		conv = convs->content;
		res = fwrite_conv(fd, conv, p_args);
		if (res < 0)
			return (res);
		n_put += res;
		convs = convs->next;
	}
	return (n_put);
}
```

리스트 출력기는 단순히 티켓 리스트를 끝까지 순회하며 각 티켓이 요구하는 출력기를 호출하고 결과를 수집한다. 중간에 개별 호출기가 오류를 반환할 경우 출력은 중지되고 오류 코드를 반환한다. 모든 출력이 성공적으로 종료될 경우 각 출력기가 보고한 출력한 문자 개수를 합산하여 반환한다.

이때 `di`, `u` 변환은 진법이 같으나 부호의 유무가 다르다. 이 둘은 매개 변수의 타입이 다른 두 함수를 준비하여 형변환이 일어나도록 하여 구분한다. `u`, `x`, `X` 변환은 모두 부호 없는 정수로 변환한다는 점에서 그 기능의 본질이 같다. 따라서 세 변환을 하나의 논리로 처리하고 그 진법(base)을 매개 변수로 제공하여 구분한다.
- `di`: `int`로 형변환, `"0123456789"`진법 사용
- `u`: `unsigned int`로 형변환, `"0123456789"`진법 사용
- `x`: `unsigned int`로 형변환, `"0123456789abcdef"`진법 사용
- `X`: `unsigned int`로 형변환, `"0123456789ABCDEF"`진법 사용

`%` 변환은 별도로 구현하지 않고 `c` 변환에 `'%'`를 입력하여 처리한다.

## 티켓 출력기

티켓 출력기는 다음과 같이 동작한다:

- 변환이 필요하지 않은 티켓이라면 `format` 문자열에서 해당 부분을 그대로 출력한다.
- 변환이 필요하다면:
  - 가변 인자에서 필요한 타입의 변수를 하나 소모한다.
  - 해당 변수를 티켓에서 요구하는대로 변환하여 문자열에 저장한다.
  - 문자열을 `STDOUT`에 `write()`한다. 이때
    - I/O 관련 오류 발생 시 음의 정수를 반환한다.
    - 오류 없이 작업 완료 시 문자열의 길이를 반환한다.

이때 빈 문자열을 생성한 후 필요한 요소를 붙여 나가며 변환을 수행하게 된다. 필요한 요소에는 다음과 같은 요소가 있다.

- 접두사 (prefix)

> 접두사는 숫자 변환(`pdiuxX`)에 출현하는 요소다. 
> 접두사는 **부호**, **16진수 표시자**, **정밀도 표시자**로 이루어진다.
> - 부호는 다음 규칙에 따라 생성한다.
>   - 음수에는 `'-'`을 표기한다.
>   - 양수일 경우, `di` (또는 `SDEC`)변환이 아닐 경우 부호를 표기하지 않는다.
>     - `di`일 경우
>       - `f_sign`이 켜졌을 시 `'+'`를 표기한다.
>       - `f_blank`가 켜졌을 시 `' '`를 표기한다.
>       - 해당되는 경우가 없을 시 부호를 표기하지 않는다.
> - 16진수 표시자는
>   - `p` 변환에서 `"0x"`로 표기한다.
>   - `xX` 변환에서 `0`이 아닌 수를 변환할 때 대소문자에 따라 `"0x"` 또는 `"0X"`로 표기한다.
> - 정밀도 표시자는 `f_precision`이 켜져 있고 숫자의 길이가 `precision`보다 짧을 경우 그 차만큼 `'0'`을 반복하여 표기한다.

- 여백 (padding)

> 여백에는 공백(`' '`)과 `'0'`의 2종류가 있다. 
> `f_minwidth` 플래그가 켜져 있고 출력할 문자열의 총 길이가 이보다 짧을 시 여백이 필요하다.
> - `f_zeropad` 플래그가 켜져 있고, `f_precision` 플래그가 꺼져 있고, `f_left` 플래그가 꺼져 있을 시 여백에 `'0'`을 채운다. 그렇지 않을 시 `' '`을 채운다.

- 주부

> 변수를 문자열로 변환한 결과이다.
> 부호나 자릿수 관련 사항은 여백과 접두사에 일임하므로 정수 변환 시 부호 여하를 막론하고 숫자만 사용하여 변환한다.
> 문자열의 경우 `f_precision`의 적용에 따라 길이를 제한한 결과이다.

이때 변환의 종류와 설정된 플래그의 조합에 따라 요소의 출력 순서가 달라지기 때문에 먼저 모든 요소의 생성을 완료한 뒤 이를 올바른 순서에 따라 병합한 후 한번에 출력하는 것이 간단한 방법이다.

### 요소 생성기

구현의 용이성을 위해 **출력기**와 별개로 **요소 생성기**를 구현한다. **요소 생성기**는 **출력기**처럼 티켓과 변수를 입력받아 상기된 규칙대로 문자열 요소를 생성한다. 다만 이를 `STDOUT`에 출력할 권한은 없으며 요소의 연결과 출력은 **출력기**가 전담한다.

- N개의 문자가 반복되는 문자열 생성기

```c
char	*cstr_nchars(int n, char c)
{
	char	*buf;
	int		i;

	buf = (char *)malloc(n + 1);
	if (!buf)
		return (NULL);
	i = 0;
	while (i < n)
	{
		buf[i] = c;
		i++;
	}
	buf[i] = '\0';
	return (buf);
}
```

- 부호 없는 숫자의 문자열 생성기

```c
int	cstr_nbr_fill_str(char *buf, t_ll nbr, char *base, int l_base)
{
	int	digit;
	int	i_digit;

	if (nbr == 0)
		return (0);
	digit = nbr % l_base;
	if (digit < 0)
		digit = -digit;
	i_digit = cstr_nbr_fill_str(buf, nbr / l_base, base, l_base);
	buf[i_digit] = base[digit];
	return (i_digit + 1);
}

char	*cstr_nbr(t_conv *conv, t_ll nbr, char *charset)
{
	int		len_nbr;
	int		len_base;
	char	*buf;

	if (nbr == 0)
	{
		if (conv->f_precision)
			return (ft_strdup(""));
		else
			return (ft_substr(charset, 0, 1));
	}
	len_base = ft_strlen(charset);
	len_nbr = ft_numlen_base(nbr, len_base);
	buf = (char *)malloc(len_nbr + 1);
	if (!buf)
		return (NULL);
	cstr_nbr_fill_str(buf, nbr, charset, len_base);
	buf[len_nbr] = '\0';
	return (buf);
}
```

> `char *charset` 변수를 통해 진법을 문자열 형태로 (예: `"0123456789abcdef"`) 제공받는다. 

- 접두사 생성기

```c
char	*cstr_sign(t_conv *conv, t_ll num)
{
	if (num >= 0)
	{
		if (conv->i_conv != SDEC)
			return (ft_strdup(""));
		if (conv->f_sign)
			return (ft_strdup(PREFIX_POS));
		else if (conv->f_blank)
			return (ft_strdup(PREFIX_BLANK));
		return (ft_strdup(""));
	}
	else
		return (ft_strdup(PREFIX_NEG));
}

int	append_hex_prefix(t_conv *conv, t_ll num, t_ll base, char **buf)
{
	if (base == 16 && conv->f_altform && num != 0)
	{
		if (conv->i_conv == LHEX && ft_strappend(buf, PREFIX_LHEX) < 0)
			return (CODE_ERROR_MALLOC);
		if (conv->i_conv == UHEX && ft_strappend(buf, PREFIX_UHEX) < 0)
			return (CODE_ERROR_MALLOC);
	}
	return (CODE_OK);
}

int	append_precision(t_conv *conv, t_ll num, t_ll base, char **buf)
{
	int		len_num;
	int		res;
	char	*temp;

	len_num = ft_numlen_base(num, base);
	if (conv->f_precision && conv->precision > len_num)
	{
		temp = cstr_nchars(conv->precision - len_num, CHARSET_LHEX[0]);
		if (!temp)
			return (CODE_ERROR_MALLOC);
		res = ft_strappend(buf, temp);
		free(temp);
		if (res < 0)
			return (CODE_ERROR_MALLOC);
	}
	return (CODE_OK);
}

char	*cstr_prefix(t_conv *conv, t_ll num, t_ll base)
{
	char	*buf;

	buf = cstr_sign(conv, num);
	if (!buf)
		return (NULL);
	if (append_hex_prefix(conv, num, base, &buf) < 0)
	{
		free(buf);
		return (NULL);
	}
	if (append_precision(conv, num, base, &buf) < 0)
	{
		free(buf);
		return (NULL);
	}
	return (buf);
}
```

- 여백 생성기

```c
char	*cstr_padding(t_conv *conv, int len_rest)
{
	int		len_pad;
	char	*padding;

	len_pad = 0;
	if (conv->f_minwidth && conv->minwidth > len_rest)
		len_pad = conv->minwidth - len_rest;
	if (conv->f_zeropad && !conv->f_precision && !conv->f_left)
		padding = cstr_nchars(len_pad, SYMBOL_ZEROPAD);
	else
		padding = cstr_nchars(len_pad, SYMBOL_BLANK);
	if (!padding)
		return (NULL);
	return (padding);
}
```

- 포인터 변수의 문자열 생성기

```c
static int	ft_ptrlen(void *ptr)
{
	unsigned char	*cursor;
	int				len;

	len = sizeof(ptr) * 2;
	cursor = (unsigned char *)(&ptr) + sizeof(ptr) - 1;
	while (len > 0)
	{
		if (*cursor / 16 != 0)
			break ;
		len--;
		if (*cursor % 16 != 0)
			break ;
		len--;
		cursor--;
	}
	return (len);
}

static void	cstr_ptr_fill_str(void *ptr, int len_ptr, char *buf)
{
	unsigned char	*cursor;
	int				i;

	cursor = (unsigned char *)(&ptr) + sizeof(ptr) - 1;
	i = -(sizeof(ptr) * 2) + len_ptr;
	while (i < len_ptr)
	{
		if (i >= 0)
			buf[i] = CHARSET_LHEX[*cursor / 16];
		i++;
		if (i >= 0)
			buf[i] = CHARSET_LHEX[*cursor % 16];
		i++;
		cursor--;
	}
	buf[len_ptr] = '\0';
}

static char	*cstr_ptr_unsigned(t_conv *conv, void *ptr, int len_ptr)
{
	char			*buf;

	if (ptr == NULL)
	{
		if (conv->f_precision)
			return (ft_strdup(""));
		else
			return (ft_substr(CHARSET_LHEX, 0, 1));
	}
	buf = (char *)malloc(len_ptr + 1);
	if (!buf)
		return (NULL);
	cstr_ptr_fill_str(ptr, len_ptr, buf);
	return (buf);
}
```

> 본 프로젝트에서 "숫자"는 `int`로 한정한다. 즉 `int`의 범위를 넘는 수를 입력하면 오버플로가 일어나는 것이 예상되는 현상이다.
> 포인터 변수를 대표할 `void *` 형식 변수는 `int`와 다른 크기를 가질 수 있기 때문에 별도의 논리를 고안하여 구현한다.
> 진법은 `"0123456789abcdef"`로 고정된다.

- 포인터 변수의 접두사 생성기

```c
static char	*cstr_ptr_prefix(t_conv *conv, int len_ptr)
{
	int		res;
	char	*buf;
	char	*temp;

	buf = ft_strdup(PREFIX_LHEX);
	if (!buf)
		return (NULL);
	if (conv->f_precision && conv->precision > len_ptr)
	{
		temp = cstr_nchars(conv->precision - len_ptr, PREFIX_BLANK[0]);
		if (!temp)
		{
			free(buf);
			return (NULL);
		}
		res = ft_strappend(&buf, temp);
		free(temp);
		if (res < 0)
			return (NULL);
	}
	return (buf);
}
```

### 요소 연결기

여백, 접두사, 주부의 연결 논리는 변화무쌍하고 복잡하다. 이것을 출력기에 포함시키기보다는 각 상황에 적합한 연결기를 고안하고 출력기가 이를 호출할 수 있도록 하는 것이 응집도 향상에 유리하다.

- 숫자 연결기

```c
char	*merge_num_buffers(t_conv *conv, char **buf)
{
	char	*merged;

	if (buf[3][0] == SYMBOL_ZEROPAD)
		merged = ft_strmerge(3, buf[1], buf[3], buf[2]);
	else
	{
		if (conv->f_left)
			merged = ft_strmerge(3, buf[1], buf[2], buf[3]);
		else
			merged = ft_strmerge(3, buf[3], buf[1], buf[2]);
	}
	return (merged);
}
```

> `diuxX` 변환에서 여백, 접두사, 주부를 연결한다.
> - 여백이 존재하고 `0`으로 이루어졌을 경우 접두사, 여백, 주부 순서로 연결한다.
> - 그렇지 않을 경우:
>   - `f_left`가 켜져 있을 경우 접두사, 주부, 여백 순서로 연결한다.
>   - `f_left`가 꺼져 있을 경우 여백, 접두사, 주부 순서로 연결한다.

- 여백 연결기

```c
int	add_padding(t_conv *conv, char **buf)
{
	int		res;
	char	*padding;
	char	*temp;

	padding = cstr_padding(conv, ft_strlen(*buf));
	if (!padding)
		return (CODE_ERROR_MALLOC);
	if (!conv->f_left)
	{
		temp = *buf;
		*buf = padding;
		padding = temp;
	}
	res = ft_strappend(buf, padding);
	free(padding);
	if (res < 0)
		return (CODE_ERROR_MALLOC);
	return (CODE_OK);
}
```

> 접두사 생성이 일어나지 않는 `s` 변환을 위한 연결기로서 여백과 주부를 연결한다. `f_left`의 적용 여부에 따라 여백, 주부 또는 주부, 여백 순서로 연결한다.

### plain 출력기

```c
int	fwrite_plain(int fd, t_conv *conv)
{
	return (write(fd, conv->s, conv->e - conv->s));
}
```

해석기가 명시한대로 `t_conv.s`부터 `t_conv_e` 전까지 `format`의 내용을 그대로 출력한다.

### c 출력기

```c
static int	fwrite_char_with_padding(int fd, t_conv *conv, int c, char *pad)
{
	unsigned char	buf;
	int				res;

	buf = c;
	if (!conv->f_left)
	{
		res = write(fd, pad, ft_strlen(pad));
		if (res < 0)
			return (res);
	}
	res = write(fd, &buf, 1);
	if (res < 0)
		return (res);
	if (conv->f_left)
	{
		res = write(fd, pad, ft_strlen(pad));
		if (res < 0)
			return (res);
	}
	return (CODE_OK);
}

int	fwrite_char(int fd, t_conv *conv, int c)
{
	int		n_put;
	int		res;
	char	*padding;

	n_put = 1;
	if (conv->f_minwidth && conv->minwidth > 1)
	{
		padding = cstr_nchars(conv->minwidth - 1, PREFIX_BLANK[0]);
		n_put += conv->minwidth - 1;
	}
	else
		padding = ft_strdup("");
	if (!padding)
		return (CODE_ERROR_MALLOC);
	res = fwrite_char_with_padding(fd, conv, c, padding);
	free(padding);
	if (res < 0)
		return (res);
	return (n_put);
}
```

- 최소 너비 플래그가 지정되어 있고 그 값이 1보다 클 시
  - 그 차만큼 공백을 출력해야 한다.
    - 좌측 정렬 플래그가 지정되어 있을 시:
      - `char` 하나를 출력하고 공백을 출력한다.
    - 그렇지 않을 시:
      - 공백을 출력하고 `char` 하나를 출력한다.
- 해당하지 않을 시 공백을 출력한다.

> s 출력기에 길이가 1인 문자열을 입력하는 방식으로 구현할 경우 `\0`을 처리할 수 없다.

### s 출력기

```c
int	fwrite_str(int fd, t_conv *conv, char *str)
{
	int		n_put;
	int		res;
	char	*buf;
	int		len_max;

	if (!str)
		str = SYMBOL_NULL;
	len_max = ft_strlen(str);
	if (conv->f_precision && conv->precision < len_max)
		len_max = conv->precision;
	buf = ft_substr(str, 0, len_max);
	if (!buf)
		return (CODE_ERROR_MALLOC);
	if (add_padding(conv, &buf) < 0)
	{
		free(buf);
		return (CODE_ERROR_MALLOC);
	}
	n_put = ft_strlen(buf);
	res = write(fd, buf, n_put);
	free(buf);
	if (res < 0)
		return (res);
	return (n_put);
}
```

- 제공받은 문자열이 `NULL`일 시 `(null)`을 대신 출력한다.
- 정밀도 플래그가 지정되어 있고 이가 문자열 길이보다 짧을 시 원 문자열을 절단하여 주부를 생성한다.
  - 그렇지 않을 시 원 문자열이 주부가 된다.
- 여백 연결기를 호출하고 결과물을 출력한다.

### p 출력기

```c
int	fwrite_ptr(int fd, t_conv *conv, void *ptr)
{
	int		res;
	char	*buf[4];
	int		len_ptr;

	len_ptr = ft_ptrlen(ptr);
	buf[1] = cstr_ptr_prefix(conv, len_ptr);
	buf[2] = cstr_ptr_unsigned(conv, ptr, len_ptr);
	buf[3] = cstr_padding(conv, ft_strlen(buf[1]) + ft_strlen(buf[2]));
	buf[0] = merge_num_buffers(conv, buf);
	if (!buf[0])
	{
		abort_fwrite(buf);
		return (CODE_ERROR_MALLOC);
	}
	res = write(fd, buf[0], ft_strlen(buf[0]));
	abort_fwrite(buf);
	if (res < 0)
		return (res);
	return (res);
}
```

- 입력받은 변수와 티켓에 따라 접두사, 주부, 여백을 생성한다.
- 연결기를 호출하고 결과를 출력한다.

### d, i 출력기

```c
int	fwrite_int(int fd, t_conv *conv, int num, char *charset)
{
	int		res;
	char	*buf[4];

	buf[1] = cstr_prefix(conv, num, ft_strlen(charset));
	buf[2] = cstr_nbr(conv, num, charset);
	buf[3] = cstr_padding(conv, ft_strlen(buf[1]) + ft_strlen(buf[2]));
	buf[0] = merge_num_buffers(conv, buf);
	if (!buf[0])
	{
		abort_fwrite(buf);
		return (CODE_ERROR_MALLOC);
	}
	res = write(fd, buf[0], ft_strlen(buf[0]));
	abort_fwrite(buf);
	if (res < 0)
		return (res);
	return (res);
}
```

- 입력받은 티켓, 변수, 진법에 따라 접두사, 주부, 여백을 생성한다.
- 연결기를 호출하고 결과를 출력한다.

### u, x, X 출력기

```c
int	fwrite_uint(int fd, t_conv *conv, t_uint num, char *charset)
{
	int		res;
	char	*buf[4];

	buf[1] = cstr_prefix(conv, num, ft_strlen(charset));
	buf[2] = cstr_nbr(conv, num, charset);
	buf[3] = cstr_padding(conv, ft_strlen(buf[1]) + ft_strlen(buf[2]));
	buf[0] = merge_num_buffers(conv, buf);
	if (!buf[0])
	{
		abort_fwrite(buf);
		return (CODE_ERROR_MALLOC);
	}
	res = write(fd, buf[0], ft_strlen(buf[0]));
	abort_fwrite(buf);
	if (res < 0)
		return (res);
	return (res);
}
```

- 논리 구조는 d, i 출력기와 같으나 매개 변수의 타입이 다르다.

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
