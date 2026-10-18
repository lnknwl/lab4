@echo off
setlocal
cd /d "%~dp0"

doxygen Doxyfile
if errorlevel 1 exit /b 1

if not exist docs\html\uml mkdir docs\html\uml
copy /y uml\chess.puml docs\html\uml\chess.puml >nul
if errorlevel 1 exit /b 1
copy /y uml\chess.svg docs\html\uml\chess.svg >nul
if errorlevel 1 exit /b 1
