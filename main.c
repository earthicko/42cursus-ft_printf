#include "libftprintf.h"

int main()
{
	char *base = "%+.5d %+.5d\n";
	//char *world = "World!";
	ft_printf(base, 42, -42);
	printf(base, 42, -42);
	return 0;
}
