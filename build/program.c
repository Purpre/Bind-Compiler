#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#ifdef _WIN32
#include <windows.h>
#endif
int main(){
char* __global_color__ = "[0m";
system("cls");
goto main;
main:
system("pause");
return 0;}