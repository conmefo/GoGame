@echo off
echo === Compiling SFML project ===
g++ -c main.cpp -I"C:\SFML-2.6.2\include"
g++ -L"C:\SFML-2.6.2\lib" main.o -o GoGame.exe -lmingw32 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-main -mwindows
echo === Running program ===
GoGame.exe
pause
