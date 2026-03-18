@echo off

set CC=clang
set CFLAGS=-Wall -g
set LIBS=-fopenmp -lgdi32 -luser32 -lwinmm
set COMMAND=%1
set SHADER=%2

if "%COMMAND%"=="" (
	set COMMAND=all
	set SHADER=.\src\shaders\test.c
)

if "%COMMAND%"=="all" goto all
if "%COMMAND%"=="clean" goto clean
if "%COMMAND%"=="build" goto build
if "%COMMAND%"=="run" goto run
goto end

:all
call :build
call :run
goto end

:clean
if exist target rmdir /s /q target
if not exist target mkdir target
echo Cleaned
goto end

:build
%CC% ^
	%CFLAGS% ^
	-o ^
	.\target\main.exe ^
	.\src\*.c ^
	.\vendor\minifb\src\*.c ^
	.\vendor\minifb\src\windows\*.c ^
	%SHADER% ^
	-I .\vendor\minifb\include ^
	-I .\vendor\minifb\src ^
	%LIBS%
echo Built
goto end

:run
echo Running
target\main.exe
goto end

:end
