@echo off

cmake -S . -B build
cmake --build build

pause
cls

cd build
Bind

pause