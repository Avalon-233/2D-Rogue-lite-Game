@echo off
setlocal
echo Installing the Microsoft Visual C++ Redistributable (x64)...
"%~dp0vc_redist.x64.exe" /install /passive /norestart
if errorlevel 1 (
  echo Runtime installation failed. Please run this file as administrator.
  pause
  exit /b 1
)
echo Runtime installation completed.
pause
