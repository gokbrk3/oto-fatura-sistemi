@echo off
echo Oto Fatura Sistemi - C++ Build Script
echo ======================================

REM Check if Qt6 is installed
if not exist "C:\Qt\6.5.0\msvc2019_64\bin\qmake.exe" (
    echo HATA: Qt6 bulunamadi!
    echo Lutfen Qt6'yi C:\Qt\6.5.0\msvc2019_64\ konumuna kurun
    pause
    exit /b 1
)

REM Create build directory
if not exist "build" mkdir build
cd build

REM Configure with CMake
echo CMake konfigurasyonu yapiliyor...
cmake .. -DCMAKE_PREFIX_PATH="C:\Qt\6.5.0\msvc2019_64" -DCMAKE_BUILD_TYPE=Release

if %ERRORLEVEL% neq 0 (
    echo HATA: CMake konfigurasyonu basarisiz!
    pause
    exit /b 1
)

REM Build the project
echo Proje derleniyor...
cmake --build . --config Release

if %ERRORLEVEL% neq 0 (
    echo HATA: Derleme basarisiz!
    pause
    exit /b 1
)

echo.
echo ======================================
echo Derleme basarili!
echo Calistirmak icin: build\bin\OtoFaturaSistemi.exe
echo ======================================
pause
