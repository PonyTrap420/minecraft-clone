@echo off
cmake -S . -B bin -G "MinGW Makefiles" -DCMAKE_C_COMPILER=gcc
cmake --build bin
if exist bin\minecraft-clone.exe (
    echo Running minecraft-clone...
    bin\minecraft-clone.exe
) else (
    echo Build failed or executable not found.
)
pause