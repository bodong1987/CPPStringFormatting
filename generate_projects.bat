@echo off
setlocal

REM set default values
set PAUSE=YES
set CPP_STD=14

:next_arg
if "%~1"=="" goto args_done
if "%~1"=="--nopause" (
    set PAUSE=NO
    shift
    goto next_arg
)
if "%~1"=="--cppstd" (
    shift
    set CPP_STD=%~2
    shift
    goto next_arg
)
shift
goto next_arg

:args_done

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

REM check for VS2008
echo Checking for Visual Studio 2008...
set "VS2008_PATH=C:\Program Files (x86)\Microsoft Visual Studio 9.0"
if exist "%VS2008_PATH%" (
    echo Visual Studio 2008 found. Generating Visual Studio 2008 project...
    cmake -G "Visual Studio 9 2008" -S %PROJECT_ROOT% -B %BUILD_DIR%\vs2008 -DCMAKE_CXX_STANDARD=%CPP_STD%
    if %errorlevel% neq 0 (
        echo Failed to generate VS2008 project
        exit /b %errorlevel%
    )
) else (
    echo Visual Studio 2008 not found. Skipping VS2008 project generation.
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