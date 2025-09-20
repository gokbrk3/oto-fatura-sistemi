# ================== START CONFIG ==================
import time
import pandas as pd
from pathlib import Path
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.chrome.service import Service
from selenium.webdriver.common.keys import Keys
from selenium.common.exceptions import TimeoutException, NoSuchElementException

# ==== Ayarlar ====
PORTAL_URL = "https://yeniportal.zirvedonusum.com/accounting/login"
EXCEL_PATH = Path(r"c:/projeler/oto fatura/test_fatura_zirve.xlsx")
# ================== END CONFIG ==================

print("✅ Doğru zirve_selenium.py dosyası çalışıyor")
print("🔄 GitHub güncelleme kontrolü - 2025-09-20 17:10:00")

# ================== START EXCEL ==================
def read_excel_data(sirket, baslik):
    df = pd.read_excel(EXCEL_PATH)
    df = df[(df["Şirket"] == sirket) & (df["Fatura Başlığı"] == baslik)]
    if df.empty:
        raise ValueError(f"Excel'de {sirket} için {baslik} bulunamadı!")
    return {
        "vergi_no": str(df.iloc[0]["Vergi No / TC"]),
        "unvan": baslik,
        "vergi_sehir": df.iloc[0]["Vergi D. Şehri"],
        "vergi_dairesi": df.iloc[0]["Vergi D."],
        "adres": df.iloc[0]["Adres"],
        "adres_sehir": df.iloc[0]["Adres Şehir"],
        "adres_ilce": df.iloc[0]["Adres İlçe"],
        "urun_adi": df.iloc[0]["Ürün Adı"],
        "miktar": df.iloc[0]["Miktar"],
        "birim": df.iloc[0]["Birim"],
        "birim_fiyat": df.iloc[0]["Birim Fiyat"],
        "kdv": df.iloc[0]["KDV %"],
        "iskonto": df.iloc[0]["İskonto %"],
        "aciklama": df.iloc[0]["Açıklama"]
    }
# ================== END EXCEL ==================

# ================== START LOGIN ==================
def login_portal(driver, username, password):
    """
    Zirve portalına giriş yapar.
    GUI'den gelen kullanıcı adı (username) ve şifre (password) ile çalışır.
    """
    driver.get("https://yeniportal.zirvedonusum.com/accounting/login")

    try:
        wait = WebDriverWait(driver, 20)

        # Kullanıcı adı inputunu bul
        try:
            user_input = wait.until(EC.presence_of_element_located((By.ID, "username")))
        except TimeoutException:
            user_input = wait.until(
                EC.presence_of_element_located((By.XPATH, "//input[@placeholder='Kullanıcı Adı']"))
            )

        # Şifre inputunu bul
        try:
            pass_input = wait.until(EC.presence_of_element_located((By.ID, "password")))
        except TimeoutException:
            pass_input = wait.until(
                EC.presence_of_element_located((By.XPATH, "//input[@placeholder='Şifre']"))
            )

        # Alanları doldur
        user_input.clear()
        user_input.send_keys(username)
        pass_input.clear()
        pass_input.send_keys(password)

        # Giriş butonu
        try:
            login_btn = wait.until(
                EC.element_to_be_clickable((By.XPATH, "//button[contains(text(),'GİRİŞ')]"))
            )
        except TimeoutException:
            login_btn = wait.until(
                EC.element_to_be_clickable((By.CSS_SELECTOR, "button.login-button"))
            )

        login_btn.click()
        print("✅ Giriş butonuna tıklandı.")

        # Giriş sonrası e-Dönüşüm menüsünün yüklenmesini bekle
        WebDriverWait(driver, 20).until(
            EC.element_to_be_clickable((By.XPATH, "//a[@href='#pagesTransformation']"))
        )
        print("✅ Portal giriş başarılı, e-Dönüşüm menüsü hazır!")

    except TimeoutException:
        print("❌ Giriş sırasında zaman aşımı oldu.")
    except NoSuchElementException as e:
        print(f"❌ Giriş elementi bulunamadı: {e}")
# ================== END LOGIN ==================

# ================== START CREATE INVOICE SIMPLE ==================
def create_invoice_simple(driver, bilgiler):
    """
    Zirve portalında fatura oluşturma sayfasına gider
    ve Excel'den gelen müşteri bilgilerine göre vergi no ile sorgulama yapar.
    """
    # Menü → e-Dönüşüm aç
    menu_edonusum = WebDriverWait(driver, 20).until(
        EC.element_to_be_clickable((By.XPATH, "//a[@href='#pagesTransformation']"))
    )
    menu_edonusum.click()

    # Menü → Fatura Oluştur
    WebDriverWait(driver, 20).until(
        EC.element_to_be_clickable((By.LINK_TEXT, "Fatura Oluştur"))
    ).click()

    # Vergi No ile arama alanını aç
    placeholder = WebDriverWait(driver, 20).until(
        EC.element_to_be_clickable((By.CLASS_NAME, "Select-placeholder"))
    )
    placeholder.click()

    # React-select input alanını bul
    search_box = WebDriverWait(driver, 20).until(
        EC.presence_of_element_located((By.XPATH, "//div[@class='Select-input']/input"))
    )

    search_box.send_keys(bilgiler["vergi_no"])

    # "Sorgulanıyor" yazısını bekle → sonra kaybolmasını bekle
    try:
        sorgu_elem = WebDriverWait(driver, 3).until(
            EC.presence_of_element_located((By.XPATH, "//*[contains(text(),'Sorgulanıyor')]"))
        )
        print("ℹ️ Sorgulama başladı:", sorgu_elem.text)

        WebDriverWait(driver, 20).until(
            EC.invisibility_of_element_located((By.XPATH, "//*[contains(text(),'Sorgulanıyor')]"))
        )
        print("✅ Sorgulama bitti, 1 saniye bekleniyor...")
        time.sleep(1)
    except:
        print("⚠️ 'Sorgulanıyor' yazısı hiç çıkmadı.")

    # Enter bas → müşteri seç
    search_box.send_keys(Keys.ENTER)

    # Eğer popup çıkarsa "Tamam"a bas
    try:
        btn_tamam = WebDriverWait(driver, 10).until(
            EC.element_to_be_clickable((By.XPATH, "//button[contains(text(),'Tamam')]"))
        )
        btn_tamam.click()
        print("✅ Onay popup → Tamam'a basıldı.")
    except:
        print("⚠️ Onay penceresi çıkmadı, müşteri yeni olabilir.")
# ================== END CREATE INVOICE SIMPLE ==================

# ================== START PRODUCT UPLOAD ==================
def upload_products_from_excel(driver, excel_path):
    """
    Zirve portalında ürünleri Excel'den yükler.
    """
    # Gizli dosya input'unu bul
    file_input = WebDriverWait(driver, 20).until(
        EC.presence_of_element_located((By.XPATH, "//input[@type='file' and contains(@accept, '.xls')]"))
    )

    # JS ile input'u görünür hale getirelim
    driver.execute_script("arguments[0].style.display = 'block'; arguments[0].style.visibility = 'visible';", file_input)

    # Dosyayı gönder
    file_input.send_keys(str(excel_path))
    print(f"📄 Excel dosyası direkt input'a gönderildi: {excel_path}")

    # Yükleme bitene kadar bekle
    WebDriverWait(driver, 30).until_not(
        EC.presence_of_element_located((By.XPATH, "//*[contains(text(),'Yükleniyor')]"))
    )
    print("✅ Ürünler Excel'den yüklendi (native pencere açılmadan).")
# ================== END PRODUCT UPLOAD ==================

# ================== START ALICI DUZENLE ==================
def alici_duzenle(driver, bilgiler):
    def clear_input_hard(el):
        # Görünecek şekilde kaydır + odakla
        driver.execute_script("arguments[0].scrollIntoView({block:'center'});", el)
        el.click()
        # 1) Klasik temizleme
        try:
            el.clear()
        except Exception:
            pass
        el.send_keys(Keys.CONTROL, "a")
        el.send_keys(Keys.DELETE)
        # 2) React için JS + event tetikleme
        driver.execute_script("""
            const el = arguments[0];
            el.value = '';
            el.dispatchEvent(new Event('input', {bubbles:true}));
            el.dispatchEvent(new Event('change', {bubbles:true}));
        """, el)
        # 3) Kısa doğrulama döngüsü
        for _ in range(2):
            val = (el.get_attribute("value") or "").strip()
            if val != "":
                el.click()
                el.send_keys(Keys.CONTROL, "a")
                el.send_keys(Keys.DELETE)
                driver.execute_script("arguments[0].value='';", el)
            else:
                break

    # Alıcı Düzenle butonuna tıkla
    btn_alici = WebDriverWait(driver, 20).until(
        EC.element_to_be_clickable((By.CSS_SELECTOR, "button.TekBtn.pull-right.edit"))
    )
    btn_alici.click()
    time.sleep(1)

    # Ünvan alanını (geçici olarak) doldur
    unvan_input = WebDriverWait(driver, 20).until(
        EC.presence_of_element_located((By.ID, "tFormattedName"))
    )
    clear_input_hard(unvan_input)
    unvan_input.send_keys(bilgiler["unvan"])

    # Vergi D. Şehir
    sehir_input = WebDriverWait(driver, 20).until(
        EC.presence_of_element_located((By.XPATH, "//label[contains(text(),'Vergi D. Şehir')]/../following-sibling::div//input"))
    )
    sehir_input.send_keys(bilgiler["vergi_sehir"])
    time.sleep(0.1)
    sehir_input.send_keys(Keys.ENTER)

    # Vergi D.
    vergi_d_input = WebDriverWait(driver, 20).until(
        EC.presence_of_element_located((By.ID, "tTaxOffice"))
    )
    vergi_d_input.send_keys(bilgiler["vergi_dairesi"])
    time.sleep(0.1)
    vergi_d_input.send_keys(Keys.ENTER)

    # Yeni Adres Ekle → popup aç
    btn_yeni_adres = WebDriverWait(driver, 20).until(
        EC.presence_of_element_located((By.XPATH, "//div[contains(@class,'modal')]//button[contains(@class,'TekBtn') and contains(@class,'ekle')]"))
    )
    driver.execute_script("arguments[0].click();", btn_yeni_adres)

    # Adres bilgileri
    adres_sehir = WebDriverWait(driver, 20).until(EC.presence_of_element_located((By.ID, "tCityName")))
    adres_sehir.send_keys(bilgiler.get("adres_sehir", "Ankara"))
    time.sleep(0.1)
    adres_sehir.send_keys(Keys.ENTER)

    adres_ilce = WebDriverWait(driver, 20).until(EC.presence_of_element_located((By.ID, "tCitySubdivisionName")))
    adres_ilce.send_keys(bilgiler.get("adres_ilce", "Çankaya"))
    time.sleep(0.1)
    adres_ilce.send_keys(Keys.ENTER)

    mahalle_input = WebDriverWait(driver, 20).until(EC.presence_of_element_located((By.NAME, "streetName")))
    mahalle_input.clear()
    mahalle_input.send_keys(bilgiler["adres"])

    # Adres ekle → tabloya ekle
    btn_adres_ekle = WebDriverWait(driver, 20).until(
        EC.presence_of_element_located((By.XPATH, "//div[contains(@class,'modal')]//button[contains(@class,'TekBtn') and contains(@class,'btn-icon') and contains(text(),'Ekle')]"))
    )
    driver.execute_script("arguments[0].click();", btn_adres_ekle)

    # Yeni eklenen adresi seç
    adres_radio = WebDriverWait(driver, 20).until(EC.presence_of_element_located((By.XPATH, "//tbody//tr[1]//input[@type='radio']")))
    driver.execute_script("arguments[0].click();", adres_radio)

    # Kaydet butonu (ilk deneme)
    btn_kaydet = WebDriverWait(driver, 20).until(
        EC.presence_of_element_located((By.XPATH, "//div[contains(@class,'modal-footer')]//button[contains(@class,'kaydet')]"))
    )
    driver.execute_script("arguments[0].click();", btn_kaydet)
    print("✅ Kaydet butonuna basıldı, kontrol ediliyor...")

    # ALERT KONTROL
    try:
        alert_box = WebDriverWait(driver, 5).until(
            EC.presence_of_element_located((By.CLASS_NAME, "s-alert-box"))
        )
        alert_text = (alert_box.text or "").replace("\n", " ").strip()
        print("⚠️ Alert bulundu:", alert_text)

        # Ad/Soyad zorunluluğunu geniş anahtar kelimelerle yakala
        low = alert_text.lower()
        if any(k in low for k in ["ad", "adı", "ad alanı", "givenname", "soyad", "soyadı", "familyname"]):
            print("➡️ İsim/Soyisim eksik → Ünvan silinecek, ad & soyad alanlarına bölünüp yazılacak.")

            # Doğru id'ler: tGivenName ve tFamilyName
            ad_input = WebDriverWait(driver, 20).until(
                EC.element_to_be_clickable((By.ID, "tGivenName"))
            )
            soyad_input = WebDriverWait(driver, 20).until(
                EC.element_to_be_clickable((By.ID, "tFamilyName"))
            )

            # Ünvan parçalanır → ilk kelime ad, geri kalan soyad
            full = (bilgiler["unvan"] or "").strip()
            if " " in full:
                ad, soyad = full.split(" ", 1)
            else:
                ad, soyad = full, "."

            # Alanları yaz
            driver.execute_script("arguments[0].scrollIntoView({block:'center'});", ad_input)
            ad_input.clear(); ad_input.send_keys(ad)
            soyad_input.clear(); soyad_input.send_keys(soyad)

            # UNVAN'I KESİN SİL (React uyumlu sert temizlik)
            clear_input_hard(unvan_input)

            # Kaydet tekrar
            btn_kaydet = WebDriverWait(driver, 20).until(
                EC.element_to_be_clickable((By.XPATH, "//div[contains(@class,'modal-footer')]//button[contains(@class,'kaydet')]"))
            )
            driver.execute_script("arguments[0].click();", btn_kaydet)
            print(f"✅ Ad = '{ad}', Soyad = '{soyad}' yazıldı. Ünvan silindi, tekrar Kaydet'e basıldı.")

            # İsteğe bağlı: Silindi mi kontrol et (modal kapanmadıysa)
            try:
                current_unvan = (unvan_input.get_attribute("value") or "").strip()
                print("ℹ️ Ünvan alanı mevcut değer:", repr(current_unvan))
            except Exception:
                pass

    except Exception:
        print("✅ Kaydet başarılı, alert çıkmadı.")
# ================== END ALICI DUZENLE ==================

# ================== START CHECK CUSTOMER ==================
def check_customer_and_edit(driver, bilgiler):
    # Vergi Dairesi alanını bul (label'e göre)
    vergi_dairesi_input = WebDriverWait(driver, 20).until(
        EC.presence_of_element_located((By.XPATH, "//label[contains(text(),'Vergi D.')]/../following-sibling::div//input"))
    )

    value = vergi_dairesi_input.get_attribute("value").strip()
    print(f"Vergi Dairesi alanı: '{value}'")

    if value == "":
        print("❌ Vergi Dairesi boş → müşteri bilgileri eksik, Alıcı Düzenle çalışacak.")
        alici_duzenle(driver, bilgiler)
    else:
        print("✅ Vergi Dairesi dolu → müşteri bulundu, Alıcı Düzenle gerek yok.")
# ================== END CHECK CUSTOMER ==================

# ================== START INVOICE NOTE ==================
def add_invoice_note(driver, aciklama_text):
    """
    Zirve portalında fatura açıklamasını ekler.
    """
    try:
        aciklama_box = WebDriverWait(driver, 20).until(
            EC.presence_of_element_located((By.ID, "note_0"))
        )
        aciklama_box.clear()
        aciklama_box.send_keys(aciklama_text)
        print(f"📝 Açıklama eklendi: {aciklama_text}")
    except Exception as e:
        print(f"❌ Açıklama eklenemedi: {e}")
# ================== END INVOICE NOTE ==================

# ================== START FINAL SAVE ==================
def save_and_close_invoice(driver):
    """
    Zirve portalında fatura için Kaydet & Kapat işlemini yapar
    ve işlem bittiğinde tarayıcıyı kapatır.
    """
    try:
        btn_kaydet = WebDriverWait(driver, 20).until(
            EC.element_to_be_clickable((
                By.XPATH,
                "//button[contains(@class,'terminate') and @title='Kaydet & Kapat']"
            ))
        )
        driver.execute_script("arguments[0].click();", btn_kaydet)
        print("✅ Kaydet & Kapat butonuna basıldı, fatura taslağa kaydedildi.")

        # Kaydettikten sonra tarayıcıyı kapat
        time.sleep(2)  # işlem tamamlansın diye küçük bekleme
        driver.quit()
        print("✅ Tarayıcı kapatıldı, yeni fatura için tekrar açılacak.")

    except Exception as e:
        print(f"❌ Kaydet & Kapat işlemi başarısız: {e}")
# ================== END FINAL SAVE ==================

# ================== START MAIN SCRIPT ==================
if __name__ == "__main__":
    sirket = "Şirket1"
    baslik = "Ahmet Yılmaz"
    bilgiler = read_excel_data(sirket, baslik)
    print("Excel'den okunan:", bilgiler)

    TEST_EXCEL_PATH = Path(r"c:/projeler/oto fatura/test_fatura_zirve.xlsx")

    service = Service(executable_path="chromedriver.exe")
    driver = webdriver.Chrome(service=service)
    driver.maximize_window()

    try:
        print("Portala giriş yapılıyor...")
        login_portal(driver, sirket)
        print("Giriş başarılı. Fatura oluşturma sayfası açılıyor...")

        # 1) Müşteri seç
        create_invoice_simple(driver, bilgiler)

        # 2) Müşteri bilgilerini kontrol et / düzenle
        check_customer_and_edit(driver, bilgiler)

        # 3) Ürünleri Excel'den yükle
        upload_products_from_excel(driver, TEST_EXCEL_PATH)

        # 4) Açıklama ekle
        try:
            aciklama_text = (bilgiler.get("aciklama") or "").strip()
            if aciklama_text and aciklama_text != "-":
                aciklama_box = WebDriverWait(driver, 20).until(
                    EC.presence_of_element_located((By.ID, "note_0"))
                )
                aciklama_box.clear()
                aciklama_box.send_keys(aciklama_text)
                print(f"📝 Açıklama eklendi: {aciklama_text}")
            else:
                print("ℹ️ Açıklama boş/atlandı.")
        except Exception as e:
            print(f"⚠️ Açıklama eklenirken sorun oldu: {e}")

        # 5) Kaydet & Kapat
        save_and_close_invoice(driver)

        print("✅ İşlem tamamlandı. Tarayıcı açık bırakıldı, sen manuel kapatabilirsin.")
        print("ℹ️ Program tarayıcı kapanana kadar açık kalacak... (kapatmak için Ctrl+C yap)")
        while True:
            time.sleep(1)  # tarayıcı kapanana kadar bekle
    except Exception as e:
        print(f"Hata: {e}")
# ================== END MAIN SCRIPT ==================