
    -lmingw32 -lsfml-system -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -mwindows

echo Copying DLL

copy "%SFML%\bin\*.dll" "%OUTDIR%\" >nul

echo Build complete
echo Output: %OUTDIR%\GoGame.exe