@echo on

set CC=clang
set CFLAGS=-Wall -g -O2
set LIBS=-fopenmp -lgdi32 -luser32 -lwinmm
set TARGET=%1
set SHADER=%2

%CC% ^
	%CFLAGS% ^
	-o ^
	.\target\main.exe ^
	.\src\*.c ^
	.\vendor\minifb\src\*.c ^
	.\vendor\minifb\src\windows\*.c ^
	-I .\vendor\minifb\include ^
	-I .\vendor\minifb\src ^
	%LIBS%

target\main.exe %TARGET% %SHADER%
	

