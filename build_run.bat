@echo on

set CC=clang
set CFLAGS=-O1 -Wall -Wextra -g

%CC% %CFLAGS% -o .\target\main.exe .\src\main.c .\src\shaders\test.c
target\main.exe
