@echo off
cd build
Bind program.bnd
gcc program.c -o program -static
program