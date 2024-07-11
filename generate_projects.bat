@echo off
setlocal

REM check command line arguments
set PAUSE=YES
set CPP_STD=14

REM check command line arguments
set PAUSE=YES
for %%a in (%*) do (
    if "%%a"=="--nopause" set PAUSE=NO
)

echo Pause = %PAUSE%
echo C++ Standard = C++ %CPP_STD%

REM set root folder
set PROJECT_ROOT=%~dp0

REM set build folder
set BUILD_DIR=%PROJECT_ROOT%Build

REM gen x86
echo Generating Visual Studio 2022 project for x86...
cmake -G "Visual Studio 17 2022" -A Win32 -S %PROJECT_ROOT% -B %BUILD_DIR%\vs2022_x86 -DCMAKE_CXX_STANDARD=%CPP_STD%
if %errorlevel% neq 0 (
    echo Failed to generate x86 project
    exit /b %errorlevel%
)

REM gen x64
echo Generating Visual Studio 2022 project for x64...
cmake -G "Visual Studio 17 2022" -A x64 -S %PROJECT_ROOT% -B %BUILD_DIR%\vs2022_x64 -DCMAKE_CXX_STANDARD=%CPP_STD%
if %errorlevel% neq 0 (
    echo Failed to generate x64 project
    exit /b %errorlevel%
)

REM check Code::Blocks exists
echo Checking for Code::Blocks...
set "CODEBLOCKS_PATH_X86=C:\Program Files (x86)\CodeBlocks\codeblocks.exe"
set "CODEBLOCKS_PATH_X64=C:\Program Files\CodeBlocks\codeblocks.exe"
set CODEBLOCKS_FOUND=0

if exist "%CODEBLOCKS_PATH_X86%" (
    set CODEBLOCKS_FOUND=1
    set "CODEBLOCKS_PATH=%CODEBLOCKS_PATH_X86%"
) else if exist "%CODEBLOCKS_PATH_X64%" (
    set CODEBLOCKS_FOUND=1
    set "CODEBLOCKS_PATH=%CODEBLOCKS_PATH_X64%"
)

if %CODEBLOCKS_FOUND% equ 1 (
    echo Code::Blocks found at %CODEBLOCKS_PATH%. Generating Code::Blocks project...
    cmake -G "CodeBlocks - MinGW Makefiles" -S %PROJECT_ROOT% -B %BUILD_DIR%\CodeBlocks -DCMAKE_CXX_STANDARD=%CPP_STD%
    if %errorlevel% neq 0 (
        echo Failed to generate Code::Blocks project
        exit /b %errorlevel%
    )
) else (
    echo Code::Blocks not found. Skipping Code::Blocks project generation.
)

echo Generation complete.
REM pause if --nopause argument was not provided
if "%PAUSE%"=="YES" pause

endlocal