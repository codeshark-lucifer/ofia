@echo off

REM Remove existing build directory to ensure a clean build
if exist build rmdir /s /q build

REM Create build directory
mkdir build

REM Generate build system with CMake (Ninja)
cmake -B build -G "Ninja"

REM Build the project
cmake --build build -j

if %errorlevel% neq 0 (
    echo Build failed!
    exit /b %errorlevel%
) else (
    echo Build successful!
)
