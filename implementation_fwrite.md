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
