@echo off
chcp 65001 >nul

if not exist build mkdir build

g++ -std=c++14 -Wall -Wextra -pedantic -finput-charset=UTF-8 -fexec-charset=CP1251 -Iinclude src\*.cpp -o build\lab4.exe
if errorlevel 1 exit /b 1

g++ -std=c++14 -Wall -Wextra -pedantic -finput-charset=UTF-8 -fexec-charset=CP1251 -Iinclude src\ChessPiece.cpp src\SlidingPiece.cpp src\Rook.cpp src\Bishop.cpp tests\*.cpp -o build\lab4-tests.exe
if errorlevel 1 exit /b 1

echo Сборка выполнена успешно
