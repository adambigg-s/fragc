@echo off

set CC=clang
set CFLAGS=-O1 -Wall -Wextra -g
set COMMAND=%1
set SHADER=%2

if "%COMMAND%"=="" set COMMAND=run

if "%COMMAND%"=="all" goto all
if "%COMMAND%"=="clean" goto clean
if "%COMMAND%"=="build" goto build
if "%COMMAND%"=="run" goto run
goto end

:all
call :clean
call :build
call :run
goto end

:clean
if exist target rmdir /s /q target
if not exist target mkdir target
goto end

:build
%CC% %CFLAGS% -o .\target\main.exe .\src\main.c .\src\shader.c %SHADER%
goto end

:run
target\main.exe
goto end

:end
