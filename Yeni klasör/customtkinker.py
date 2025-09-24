import customtkinter as ctk
from tkinter import messagebox

def giris():
    kullanici = entry_kullanici.get()
    sifre = entry_sifre.get()
    messagebox.showinfo("Giriş", f"Kullanıcı girişi: {kullanici}")

ctk.set_appearance_mode("dark")  # "light" da olabilir
ctk.set_default_color_theme("blue")

root = ctk.CTk()
root.title("CustomTkinter Giriş")
root.geometry("300x200")

ctk.CTkLabel(root, text="Kullanıcı Adı:").pack(pady=5)
entry_kullanici = ctk.CTkEntry(root)
entry_kullanici.pack(pady=5)

ctk.CTkLabel(root, text="Şifre:").pack(pady=5)
entry_sifre = ctk.CTkEntry(root, show="*")
entry_sifre.pack(pady=5)

ctk.CTkButton(root, text="Giriş", command=giris).pack(pady=10)

root.mainloop()
