#include "ft_printf.h"
#include <stdio.h>
#include <limits.h>

int main(){
	char *base = "%#x\n";
	int num = 0;
	int ret1 = ft_printf(base, num);
	int ret2 = printf(base, num);
	printf("ret %d %d\n", ret1, ret2);
	// system("leaks main");

	// TEST(15, print(" %04d ", -14));
	// TEST(16, print(" %05d ", -15));
	// TEST(17, print(" %06d ", -16));
	return 0;
}