LightRogue - Windows x64 Release package
========================================

Supported target: 64-bit Windows 10/11 computers.

How to run
----------
1. Keep all files and the resource folder in this same directory.
2. Double-click RunGame.bat or LightRogue.exe.
3. The required Microsoft C++ runtime DLLs are included beside the EXE.
   If a computer policy blocks app-local DLL loading, run InstallRuntime.bat
   once (administrator permission may be requested).

Contents
--------
- LightRogue.exe: Release executable
- sfml-*-3.dll: SFML runtime libraries
- msvcp140.dll, vcruntime140.dll, vcruntime140_1.dll: app-local Microsoft C++ runtime libraries
- resource\: game images, sounds and font resources required at runtime
- vc_redist.x64.exe: official Microsoft Visual C++ Redistributable installer
- SHA256SUMS.txt: SHA-256 hashes for integrity checking

Notes
-----
- This is a Windows x64 build. It does not run on macOS, Linux, 32-bit Windows,
  or ARM-only Windows without a corresponding build.
- Do not move LightRogue.exe away from its DLL files or resource folder.
