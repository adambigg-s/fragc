@echo on

set CC=clang
set CFLAGS=-O2 -Wall -Wextra

%CC% %CFLAGS% -o .\target\main.exe .\src\main.c .\src\shaders\test.c
target\main.exe
