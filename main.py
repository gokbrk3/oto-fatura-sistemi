#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Ana Python dosyası
Bu dosya yeni projeler için şablon olarak kullanılır.
"""

import os
import sys
from datetime import datetime

def main():
    """Ana fonksiyon"""
    print("Python Proje Şablonu")
    print("=" * 50)
    print(f"Tarih: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
    print(f"Python sürümü: {sys.version}")
    print(f"Çalışma dizini: {os.getcwd()}")
    print("=" * 50)
    print("Proje başlatıldı!")

if __name__ == "__main__":
    main()
