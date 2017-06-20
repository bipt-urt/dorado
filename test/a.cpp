#include<stdio.h>
#define ning
int main()
{
 #ifdef ning
 printf("123");
 #else
  printf("abc");
 #endif
 printf("\n hello");
 return 0;
}
