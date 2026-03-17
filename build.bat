@echo off

set CC=clang
set CFLAGS=-O3 -Wall -g -fopenmp
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
%CC% %CFLAGS% -o .\target\main.exe .\src\main.c .\src\shader_impl.c .\src\math_impl.c %SHADER%
echo Built
goto end

:run
echo Running
target\main.exe
goto end

:end
