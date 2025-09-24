# GUI dosyasını temizleme scripti
import os

try:
    # gui.py dosyasını oku
    with open('gui.py', 'r', encoding='utf-8') as f:
        content = f.read()
    
    # indir_secilen_faturalar fonksiyonundan sonraki kısmı bul
    marker = "def indir_secilen_faturalar():"
    if marker in content:
        # Bu fonksiyondan önceki kısmı al
        before_func = content.split(marker)[0]
        
        # indir_secilen_faturalar fonksiyonunu oluştur
        func_code = '''def indir_secilen_faturalar():
    """Seçilen faturaları indir"""
    global fatura_kes_sube_combo, fatura_kes_personel_entry, fatura_kes_islem_turu_combo, fatura_indirme_aktif, fatura_indirme_kuyrugu
    
    try:
        # E-Fatura tablosundan seçilenleri al
        efatura_selected = efatura_table.selection()
        earsiv_selected = earsiv_table.selection()
        
        if not efatura_selected and not earsiv_selected:
            log_yaz("⚠️ Lütfen indirmek istediğiniz faturaları seçin")
            return
        
        log_yaz(f"🔍 {len(efatura_selected)} E-Fatura, {len(earsiv_selected)} E-Arşiv seçildi")
        
        # O anki GUI değerlerini yakala
        sube_degeri = fatura_kes_sube_combo.get().strip()
        personel_degeri = fatura_kes_personel_entry.get().strip()
        islem_turu_degeri = fatura_kes_islem_turu_combo.get().strip()
        
        # Seçilen faturaları kuyruğa ekle
        fatura_indirme_kuyrugu.append({
            'efatura_selected': efatura_selected,
            'earsiv_selected': earsiv_selected,
            'sube_degeri': sube_degeri,
            'personel_degeri': personel_degeri,
            'islem_turu_degeri': islem_turu_degeri,
            'timestamp': time.time()
        })
        
        log_yaz(f"📋 Fatura indirme kuyruğa eklendi (Personel: {personel_degeri}, İşlem: {islem_turu_degeri}). Kuyruk sırası: {len(fatura_indirme_kuyrugu)}")
        
        # Eğer şu an işlem yapılmıyorsa kuyruğu başlat
        if not fatura_indirme_aktif:
            threading.Thread(target=process_fatura_indirme_kuyrugu, daemon=True).start()
        
    except Exception as e:
        log_yaz(f"❌ Fatura indirme hatası: {e}")

print("✅ GUI dosyası çalışıyor")
print("🔄 GitHub güncelleme kontrolü - 2025-09-20 17:15:00")

if __name__ == "__main__":
    gui_main()
    tk.mainloop()
'''
        
        # Temiz dosyayı oluştur
        clean_content = before_func + func_code
        
        # Yedek oluştur
        os.rename('gui.py', 'gui_backup_temp.py')
        
        # Temiz dosyayı yaz
        with open('gui.py', 'w', encoding='utf-8') as f:
            f.write(clean_content)
        
        print("✅ gui.py dosyası temizlendi")
        print("📁 Yedek: gui_backup_temp.py")
        
    else:
        print("❌ Marker bulunamadı")

except Exception as e:
    print(f"❌ Hata: {e}")
