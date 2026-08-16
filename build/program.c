#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#ifdef _WIN32
#include <windows.h>
#endif
int main(){
char* __global_color__ = "[0m";
goto main;
main:
printf("%s%d[0m",__global_color__, 2);
system("pause");
return 0;}