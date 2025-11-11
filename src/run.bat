@echo off
echo === Compiling SFML project ===
g++ -c main.cpp Board.cpp Game.cpp GameUI.cpp GameView.cpp -I"..\SFML-2.6.2\include"
g++ -L"..\SFML-2.6.2\lib" main.o Board.o Game.o GameUI.o GameView.o -o GoGame.exe -lmingw32 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-main -mwindows
echo === Running program ===
GoGame.exe
pause
