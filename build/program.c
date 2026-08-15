#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#ifdef _WIN32
#include <windows.h>
#endif
int main(){
char* __global_color__ = "[0m";
SetConsoleOutputCP(CP_UTF8);
SetConsoleCP(CP_UTF8);
system("cls");
char __command__[100];
snprintf(__command__, sizeof(__command__), "title %s","Bind - Missao Dynamic Labels");
system(__command__);
__global_color__="[36m";
int cont = 0;
int a = 0;
int b = 0;
int i = 0;
int temp = 0;
int n = 0;
goto main;
int end_imprimir_linha_assigned;
imprimir_linha:
printf("%s%s[0m\n",__global_color__, "----------------------------------------");
if (end_imprimir_linha_assigned == 1) goto ret2;
if (end_imprimir_linha_assigned == 2) goto ret3;
if (end_imprimir_linha_assigned == 3) goto ret4;
int end_imprimir_titulo_assigned;
imprimir_titulo:
__global_color__="[33m";
printf("%s%s[0m\n",__global_color__, ">>> MISSAO DYNAMIC LABELS INICIADA <<<");
__global_color__="[36m";
if (end_imprimir_titulo_assigned == 1) goto ret1;
int end_imprimir_numero_assigned;
imprimir_numero:
__global_color__="[32m";
printf("%s%s[0m",__global_color__, "-> Numero: ");
printf("%s%d[0m\n",__global_color__, n);
__global_color__="[36m";
if (end_imprimir_numero_assigned == 1) goto ret_fib;
int end_countdown_assigned;
countdown:
cont = 5;
loop_countdown:
__global_color__="[35m";
printf("%s%s[0m",__global_color__, "Contagem regressiva: ");
printf("%s%d[0m\n",__global_color__, cont);
Sleep(600);
if (cont==0) goto fim_countdown;
cont = cont - 1;
goto loop_countdown;
fim_countdown:
__global_color__="[31m";
printf("%s%s[0m\n",__global_color__, ">>> LANCAMENTO! <<<");
__global_color__="[36m";
if (end_countdown_assigned == 1) goto apos_countdown;
int end_fibonacci_assigned;
fibonacci:
a = 0;
b = 1;
i = 1;
temp = 0;
n = 0;
__global_color__="[33m";
printf("%s%s[0m\n",__global_color__, "Sequencia de Fibonacci:");
__global_color__="[37m";
loop_fib:
n = a;
end_imprimir_numero_assigned = 1;
goto imprimir_numero;
ret_fib:
temp = a + b;
a = b;
b = temp;
i = i + 1;
if (i>10) goto fim_fib;
goto loop_fib;
fim_fib:
__global_color__="[36m";
if (end_fibonacci_assigned == 1) goto apos_fib;
main:
end_imprimir_titulo_assigned = 1;
goto imprimir_titulo;
ret1:
end_imprimir_linha_assigned = 1;
goto imprimir_linha;
ret2:
end_countdown_assigned = 1;
goto countdown;
apos_countdown:
end_imprimir_linha_assigned = 2;
goto imprimir_linha;
ret3:
end_fibonacci_assigned = 1;
goto fibonacci;
apos_fib:
end_imprimir_linha_assigned = 3;
goto imprimir_linha;
ret4:
__global_color__="[32m";
printf("%s%s[0m\n",__global_color__, "Missao concluida com sucesso!");
__global_color__="[37m";
printf("%s%s[0m\n",__global_color__, "Dynamic Labels exploradas: reutilizacao de funcoes + retornos dinamicos");
system("pause");
return 0;}