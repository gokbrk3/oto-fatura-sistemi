#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Cursor'da otomatik commit/push sistemi
Bu script dosya değişikliklerini izler ve otomatik commit/push yapar
"""

import os
import subprocess
import time
from datetime import datetime
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler

class AutoCommitHandler(FileSystemEventHandler):
    """Dosya değişikliklerini izleyen handler"""
    
    def __init__(self):
        self.last_commit = 0
        self.commit_interval = 300  # 5 dakika
        
    def on_modified(self, event):
        """Dosya değişikliği olduğunda çalışır"""
        if event.is_directory:
            return
            
        # Python dosyaları ve önemli dosyalar için
        if event.src_path.endswith(('.py', '.md', '.json', '.txt', '.xlsx')):
            self.auto_commit()
    
    def auto_commit(self):
        """Otomatik commit yapar"""
        current_time = time.time()
        
        # Çok sık commit yapmayı önle
        if current_time - self.last_commit < self.commit_interval:
            return
            
        try:
            # Git status kontrol et
            result = subprocess.run(['git', 'status', '--porcelain'], 
                                  capture_output=True, text=True, cwd=os.getcwd())
            
            if not result.stdout.strip():
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
            
            print(f"✅ Otomatik commit/push: {timestamp}")
            self.last_commit = current_time
            
        except Exception as e:
            print(f"❌ Hata: {e}")

def start_auto_commit():
    """Otomatik commit sistemini başlatır"""
    print("🔄 Otomatik commit sistemi başlatılıyor...")
    print("📁 İzlenen dosyalar: .py, .md, .json, .txt, .xlsx")
    print("⏰ Commit aralığı: 5 dakika")
    print("🛑 Durdurmak için Ctrl+C")
    
    event_handler = AutoCommitHandler()
    observer = Observer()
    observer.schedule(event_handler, path='.', recursive=True)
    observer.start()
    
    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        observer.stop()
        print("\n🛑 Otomatik commit sistemi durduruldu.")
    
    observer.join()

if __name__ == "__main__":
    start_auto_commit()
