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
