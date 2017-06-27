#include "stdio.h"

struct banana;

int test = 1;
int y = 2;

int add(int, int);

int add(int _x, int _y)
{
	return _x + _y;
}

int main(int argc, char* argv[])
{
	int x = 3;
	int y = 2;
	printf(add(x, y));
	return 0;
}

struct banana
{
	int weight;
	int width;
}
