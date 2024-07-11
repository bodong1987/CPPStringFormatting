@echo off
setlocal

REM set build folder
set PROJECT_ROOT=%~dp0
set BUILD_DIR=%PROJECT_ROOT%Build

echo search msbuild.exe...
REM define search paths
set "MSBUILD_PATHS="C:\Program Files\Microsoft Visual Studio\2022\Enterprise\MSBuild\Current\Bin\amd64";"C:\Program Files\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\amd64";"C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\amd64";"C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\MSBuild\Current\Bin\amd64";"C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\MSBuild\Current\Bin\amd64";"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\amd64""

REM find msbuild.exe
for %%p in (%MSBUILD_PATHS%) do (
    if exist %%~p\msbuild.exe (
        set "MSBUILD_EXE=%%~p\msbuild.exe"
        goto found
    )
)

echo Unable to find msbuild.exe
exit /b 1

:found
echo Found msbuild.exe at %MSBUILD_EXE%

echo generate projects with C++ 14
REM call generate_projects.bat
call generate_projects.bat --nopause --cppstd 14

REM compile with different C++ standards
echo Compiling with C++ 14...
"%MSBUILD_EXE%" %BUILD_DIR%\vs2022_x64\CPPStringFormatting.sln /p:PlatformToolset=v143
if %errorlevel% neq 0 (
    echo Failed to compile with C++ 14
    exit /b %errorlevel%
)

echo generate projects with C++ 17
REM call generate_projects.bat
call generate_projects.bat --nopause --cppstd 17

echo Compiling with C++ 17...
"%MSBUILD_EXE%" %BUILD_DIR%\vs2022_x64\CPPStringFormatting.sln /p:PlatformToolset=v143
if %errorlevel% neq 0 (
    echo Failed to compile with C++ 17
    exit /b %errorlevel%
)

echo generate projects with C++ 20
REM call generate_projects.bat
call generate_projects.bat --nopause --cppstd 20

echo Compiling with C++ 20...
"%MSBUILD_EXE%" %BUILD_DIR%\vs2022_x64\CPPStringFormatting.sln /p:PlatformToolset=v143
if %errorlevel% neq 0 (
    echo Failed to compile with C++ 20
    exit /b %errorlevel%
)

echo Compilation complete.
endlocal