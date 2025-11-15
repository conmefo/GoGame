@echo off
setlocal

echo Building Project

:: Paths
set SRC=src
set SFML=SFML-2.6.2
set OUTDIR=build


:: Compile + link in ONE command
g++ ^
    "%SRC%\main.cpp" ^
    "%SRC%\Board.cpp" ^
    "%SRC%\Game.cpp" ^
    "%SRC%\GameUI.cpp" ^
    "%SRC%\GameView.cpp" ^
    -I"%SFML%\include" ^
    -L"%SFML%\lib" ^
    -o "%OUTDIR%\GoGame.exe" ^
    -lmingw32 -lsfml-system -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -mwindows

echo Copying DLL

copy "%SFML%\bin\*.dll" "%OUTDIR%\" >nul

echo Build complete
echo Output: %OUTDIR%\GoGame.exe

"%OUTDIR%\GoGame.exe"

pause
