# 발상

```c
int ft_printf(const char *format, ...)
```

`format`부터 `\0`을 발견할 때까지 문자를 순차적으로 탐색한다. `%`가 아닌 문자는 `STDOUT`에 그대로 출력한다. `%`를 마주칠 시 형식 문자열 해석기(이하 해석기)를 시작한다. 규격에 맞지 않는 문자 때문에 해석이 실패한 경우 도중의 결과를 폐기하고 문제가 되는 문자 다음부터 탐색을 재개한다. 해석이 성공했다면 가변 인자에서 그 형식이 요구하는대로 데이터를 입력받아 문자열로 변환한 후 `STDOUT`에 출력한다.

이때 2가지 선택지가 존재한다.

- 해석->변환->출력 과정을 반복한다.
  - `format`의 모든 문자에 대하여 차례대로:
    - `%`가 아닌 문자는 만날 때마다 `STDOUT`에 출력한다.
    - `%`를 만날 시:
      - 해석을 시도한다.
      - 가변 인자에서 데이터를 수집한다.
      - 문자열로 변환한다.
      - `STDOUT`에 출력한다.

- 문자열 전체의 해석을 마치고, 한꺼번에 변환해서 출력한다.
  - `format`의 모든 문자에 대하여 차례대로:
    - `%`가 아닌 문자가 어디서 어디까지 있는지 기록해둔다.
    - `%`를 만날 시:
      - 해석을 시도한다.
      - 어떤 변환을 어떻게 해야 하는지 기록해둔다.
  - 기록해둔 자료를 바탕으로:
    - 단순 문자열이라면 단순히 출력한다.
    - 변환이 필요하다면 가변 인자에서 데이터를 수집한다.
      - 문자열로 변환한다.
      - `STDOUT`에 출력한다.

전자는 `format`의 문자를 순회하는 반복문 안에서 해석->변환->출력 과정이 모두 일어나야 하기 때문에 제어 구조가 길고 에러 핸들링이 복잡하다. 반면에 후자의 방식은 첫 반복문에서는 해석만 하고, 두번째 반복문에서는 변환 및 출력만 담당하기 때문에 첫 반복문에서는 가변 인자에 접근할 필요가 없고 파일 I/O 관련 에러가 발생하지 않는다. 본 프로젝트에서는 후자의 구조를 채택한다.

# 세부 모듈 고안

`ft_printf`의 구현에는 크게 2개 기능이 필요하다.

- `format` 문자열 해석: **해석기**
- 데이터 변환 및 출력: **출력기**

## 해석기

`format`을 여러 부분으로 나누어 어느 부분이 형식 문자열이고 어느 부분이 그대로 출력되어야 하는지, 그 부분의 개수는 몇 개이며 각 형식 지정자별로 어떤 플래그를 고려하여 변환을 수행해야 하는지 파악이 필요하다. 다음과 같은 문자열을 보자.

```c
"Hello, %-7.5s!"
```

1. `Hello, ` 문자열
2. `s` 변환 지정자, 왼쪽 정렬, 최소 너비 7, 정밀도 5
3. `!` 문자열

format을 3개의 부분으로 나누어 처리해야 한다. 이때 각 부분은 다음과 같은 정보를 가진다.

> 1. 변환의 종류
>    1. 변환 없음(이하 `plain`), `c`, `s`, `p`, `d`, `i`, `u`, `x`, `X`, `%`
> 2. 원본 `format`에서 차지하는 부분
> 3. `#` **"alt form"** 플래그 활성화 여부
> 4. `0` **"zeropad"** 플래그 활성화 여부
> 5. `-` **"left"** 플래그 활성화 여부
> 6. ` ` **"blank"** 플래그 활성화 여부
> 7. `+` **"sign"** 플래그 활성화 여부
> 8. 최소 너비 **"minimum field width"** 활성화 여부
> 9. 정밀도 **"precision"** 활성화 여부
> 10. 최소 너비의 값
> 11. 정밀도의 값

이러한 정보를 기록한 변수를 티켓이라고 명명한다. 해석기는 `format` 문자열을 해석하여 필요한 티켓을 필요한 개수만큼 발부한다. 

위 문자열은 대략 다음과 같이 해석될 것이다.

1. `plain` 변환, `H`부터 `%` 전까지, 모든 플래그 비활성화
2. `s` 변환, left 플래그 활성화, 최소 너비 활성화, 정밀도 활성화, 최소 너비 7, 정밀도 5
3. `plain` 변환, `!`부터 `\0` 전까지, 모든 플래그 비활성화

## 출력기

출력기는 가변 인자로 공급받은 변수를 티켓에서 지시하는 대로 문자열 형태로 변환한 후 이를 `STDOUT`에 실제로 출력하는 임무를 맡는다. 오류가 발생할 경우 음의 정수를 반환하고, 그렇지 않을 경우 `STDOUT`에 출력한 문자의 개수를 반환한다.

보다 간단한 I/O 에러 처리를 위해 `write()` 호출은 최소한으로 이루어져야 한다. 각 출력기는 출력할 내용을 문자열의 형태로 합성해 낸 후 최후에 `write()`를 1번 호출하고, 그 반환값을 그대로 반환한다.
