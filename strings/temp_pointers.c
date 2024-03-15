#include <stdio.h>

typedef struct s_num
{
	int num;
} t_num;

void fun(int *x)
{
	(*x)++;
	printf("fun: x=%d\n",*x);
}

void fun_mod(int x)
{
	x++;
	printf("fun_mod : x=%d\n",x);
	fun(&x);
	printf("fun_mod: x=%d\n",x);
}

void modify_struct(t_num str)
{
	str.num = 10;
}

int main()
{
	int x;
	t_num *pnt;
	t_num pnt2;

	pnt->num = 10;
	pnt2.num = 0;
	x = 1;
	printf("x=%d\n",x);
	fun(&x);
	printf("x=%d\n",x);
	fun_mod(x);
	printf("x=%d\n",x);
	printf("-----  struct test ----\n");
	printf("num= %d\n", pnt->num);
	fun(&(pnt->num));
	printf("num= %d\n", pnt->num);
	printf("-----  struct test 2----\n");
	printf("num= %d\n", pnt2.num);
	modify_struct(pnt2);
	printf("num= %d\n", pnt2.num);
	return (0);
}
