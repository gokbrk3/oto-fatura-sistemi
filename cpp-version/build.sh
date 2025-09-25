#!/bin/bash

echo "Oto Fatura Sistemi - C++ Build Script"
echo "======================================"

# Check if Qt6 is installed
if ! command -v qmake &> /dev/null; then
    echo "HATA: Qt6 bulunamadı!"
    echo "Lütfen Qt6'yı kurun:"
    echo "Ubuntu/Debian: sudo apt install qt6-base-dev"
    echo "Fedora: sudo dnf install qt6-qtbase-devel"
    echo "macOS: brew install qt6"
    exit 1
fi

# Create build directory
mkdir -p build
cd build

# Configure with CMake
echo "CMake konfigürasyonu yapılıyor..."
cmake .. -DCMAKE_BUILD_TYPE=Release

if [ $? -ne 0 ]; then
    echo "HATA: CMake konfigürasyonu başarısız!"
    exit 1
fi

# Build the project
echo "Proje derleniyor..."
make -j$(nproc)

if [ $? -ne 0 ]; then
    echo "HATA: Derleme başarısız!"
    exit 1
fi

echo ""
echo "======================================"
echo "Derleme başarılı!"
echo "Çalıştırmak için: ./OtoFaturaSistemi"
echo "======================================"
