@echo off
setlocal

REM 定义子模块路径和URL
set SUBMODULE1_PATH=UnitTests/External/googletest
set SUBMODULE1_URL=https://github.com/google/googletest.git

set SUBMODULE2_PATH=Benchmark/External/Celero
set SUBMODULE2_URL=https://github.com/DigitalInBlue/Celero.git

REM 强制取消初始化子模块
echo Deinitializing submodules...
git submodule deinit -f -- %SUBMODULE1_PATH%
git submodule deinit -f -- %SUBMODULE2_PATH%

REM 删除子模块目录和 .git 中的子模块配置
echo Removing submodule directories and configurations...
rd /s /q %SUBMODULE1_PATH%
rd /s /q %SUBMODULE2_PATH%
git rm -f %SUBMODULE1_PATH%
git rm -f %SUBMODULE2_PATH%
rd /s /q .git\modules\%SUBMODULE1_PATH%
rd /s /q .git\modules\%SUBMODULE2_PATH%

REM 重新添加子模块
echo Re-adding submodules...
git submodule add --force %SUBMODULE1_URL% %SUBMODULE1_PATH%
git submodule add --force %SUBMODULE2_URL% %SUBMODULE2_PATH%

REM 强制更新子模块
echo Updating submodules...
git submodule update --init --recursive --force

echo Submodules have been reset and updated.
endlocal
pause