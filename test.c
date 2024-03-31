#include <stdio.h>

int main()
{
	int a[] = {0,1,2};
	for (int i = 0; i < 3; i+=2)
	{
		printf("%d\n",  *&*&*&a[*&*&*&a[i[*&*&*&a+i[*&*&*&a]*0+--i*0]]]);
		while (0);
	}
}