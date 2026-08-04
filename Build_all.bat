@echo off
echo =========================================
echo Building WinCEControl for all platforms
echo =========================================

set SOLUTION_DIR=%~dp0
set BUILD_DIR=%SOLUTION_DIR%build
set BIN_DIR=%SOLUTION_DIR%bin

if not exist %BIN_DIR% mkdir %BIN_DIR%

REM ARM (Release)
echo Building ARM Release...
msbuild %SOLUTION_DIR%WinCEControl.sln /p:Configuration=Release /p:Platform=ARM /p:OutDir=%BIN_DIR%\ARM\
if %errorlevel% neq 0 goto error

REM MIPS (Release)
echo Building MIPS Release...
msbuild %SOLUTION_DIR%WinCEControl.sln /p:Configuration=Release /p:Platform=MIPS /p:OutDir=%BIN_DIR%\MIPS\
if %errorlevel% neq 0 goto error

REM SH4 (Release)
echo Building SH4 Release...
msbuild %SOLUTION_DIR%WinCEControl.sln /p:Configuration=Release /p:Platform=SH4 /p:OutDir=%BIN_DIR%\SH4\
if %errorlevel% neq 0 goto error

REM x86 (Release)
echo Building x86 Release...
msbuild %SOLUTION_DIR%WinCEControl.sln /p:Configuration=Release /p:Platform=x86 /p:OutDir=%BIN_DIR%\x86\
if %errorlevel% neq 0 goto error

echo =========================================
echo Build successful!
echo Binaries in: %BIN_DIR%
echo =========================================
pause
exit 0

:error
echo =========================================
echo BUILD FAILED!
echo Check errors above.
echo =========================================
pause
exit 1
