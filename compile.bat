@echo off
echo ============================================
echo   City Scenery - OpenGL Compiler
echo ============================================
echo Compiling...

g++ main.cpp -o city_scenery.exe -lopengl32 -lglu32 -lglut32

if %errorlevel% neq 0 (
    echo.
    echo [ERROR] Compilation failed!
    pause
    exit /b %errorlevel%
)

echo.
echo [SUCCESS] Compiled successfully!
echo Launching City Scenery...
echo.
echo Controls:
echo   N   = Toggle Night / Day mode
echo   ESC = Quit
echo.
city_scenery.exe
pause
