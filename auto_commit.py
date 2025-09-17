#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Otomatik Git commit/push scripti
Bu script dosya değişikliklerini otomatik commit eder ve GitHub'a push yapar
"""

import os
import subprocess
import time
from datetime import datetime

def git_commit_push():
    """Otomatik commit ve push yapar"""
    try:
        # Git status kontrol et
        result = subprocess.run(['git', 'status', '--porcelain'], 
                              capture_output=True, text=True, cwd=os.getcwd())
        
        if not result.stdout.strip():
            print("Değişiklik yok, commit gerekmiyor.")
            return
        
        # Değişiklikleri ekle
        subprocess.run(['git', 'add', '.'], cwd=os.getcwd())
        
        # Commit mesajı oluştur
        timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        commit_message = f"Auto commit: {timestamp}"
        
        # Commit yap
        subprocess.run(['git', 'commit', '-m', commit_message], cwd=os.getcwd())
        
        # Push yap
        subprocess.run(['git', 'push', 'origin', 'main'], cwd=os.getcwd())
        
        print(f"✅ Otomatik commit/push tamamlandı: {timestamp}")
        
    except Exception as e:
        print(f"❌ Hata: {e}")

if __name__ == "__main__":
    git_commit_push()
