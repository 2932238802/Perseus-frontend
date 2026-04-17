@echo off
echo ==== Starting Pyright Installation ====
where npm >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo Error: npm is not installed. Please install Node.js first.
    exit /b 1
)
echo Installing pyright globally via npm...
call npm install -g pyright
echo ==== Installation Finished ====
