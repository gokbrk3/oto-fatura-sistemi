import customtkinter as ctk
from tkinter import messagebox

def urun_ekle():
    ad = entry_ad.get()
    fiyat = entry_fiyat.get()
    stok = entry_stok.get()

    if not ad or not fiyat or not stok:
        messagebox.showwarning("Eksik Bilgi", "Tüm alanları doldurun!")
        return
    
    try:
        fiyat = float(fiyat)
        stok = int(stok)
    except ValueError:
        messagebox.showerror("Hata", "Fiyat sayı olmalı, stok tam sayı olmalı!")
        return

    liste.insert("end", f"{ad} - {fiyat}₺ - Stok: {stok}")
    entry_ad.delete(0, "end")
    entry_fiyat.delete(0, "end")
    entry_stok.delete(0, "end")

ctk.set_appearance_mode("dark")
ctk.set_default_color_theme("blue")

root = ctk.CTk()
root.title("Ürün Yönetimi")
root.geometry("400x400")

ctk.CTkLabel(root, text="Ürün Adı:").pack(pady=5)
entry_ad = ctk.CTkEntry(root)
entry_ad.pack(pady=5)

ctk.CTkLabel(root, text="Fiyat:").pack(pady=5)
entry_fiyat = ctk.CTkEntry(root)
entry_fiyat.pack(pady=5)

ctk.CTkLabel(root, text="Stok:").pack(pady=5)
entry_stok = ctk.CTkEntry(root)
entry_stok.pack(pady=5)

ctk.CTkButton(root, text="Kaydet", command=urun_ekle).pack(pady=10)

liste = ctk.CTkTextbox(root, width=300, height=150)
liste.pack(pady=10)

root.mainloop()
