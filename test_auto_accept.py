#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Test dosyası - Auto Accept ayarlarını test etmek için
"""

import os
import sys
from datetime import datetime

def main():
    """Ana fonksiyon"""
    print("🚀 Auto Accept Test Dosyası")
    print(f"📅 Tarih: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
    print("✅ Cursor ayarları başarıyla uygulandı!")
    
    # Test değişkenleri
    test_data = {
        "name": "Test Kullanıcı",
        "age": 25,
        "city": "İstanbul"
    }
    
    # Test fonksiyonu
    def process_data(data):
        """Veri işleme fonksiyonu"""
        result = f"Merhaba {data['name']}, {data['age']} yaşındasınız ve {data['city']} şehrinde yaşıyorsunuz."
        return result
    
    # Sonuç
    message = process_data(test_data)
    print(f"📝 Mesaj: {message}")
    
    return True

if __name__ == "__main__":
    success = main()
    if success:
        print("🎉 Test başarılı!")
    else:
        print("❌ Test başarısız!")



