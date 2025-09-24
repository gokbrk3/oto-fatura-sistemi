import sys
from PySide6.QtWidgets import (
    QApplication, QWidget, QLabel, QLineEdit, QPushButton, QVBoxLayout, 
    QHBoxLayout, QTableWidget, QTableWidgetItem, QComboBox, QGroupBox
)

class ERP(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Mini ERP - PySide6")
        self.setGeometry(100, 100, 1000, 600)

        main_layout = QVBoxLayout()  # Ana dikey layout

        # === ÜST MENÜ ===
        top_menu = QHBoxLayout()
        for name in ["Ana Sayfa", "Müşteri", "Ürünler", "Satışlar", "Yönetim"]:
            btn = QPushButton(name)
            btn.setFixedHeight(40)
            top_menu.addWidget(btn)
        main_layout.addLayout(top_menu)

        # === ORTA ALAN ===
        middle_layout = QHBoxLayout()

        # SOL FORM
        form_group = QGroupBox("Müşteri Bilgileri")
        form_layout = QVBoxLayout()

        self.ad_entry = QLineEdit()
        self.tel_entry = QLineEdit()
        self.il_combo = QComboBox()
        self.il_combo.addItems(["Seçiniz", "ADANA", "ANKARA", "İSTANBUL"])
        self.durum_combo = QComboBox()
        self.durum_combo.addItems(["NORMAL", "GÜVENİLİR", "PROBLEM"])

        form_layout.addWidget(QLabel("Ad Soyad:"))
        form_layout.addWidget(self.ad_entry)
        form_layout.addWidget(QLabel("Telefon:"))
        form_layout.addWidget(self.tel_entry)
        form_layout.addWidget(QLabel("İl:"))
        form_layout.addWidget(self.il_combo)
        form_layout.addWidget(QLabel("Durum:"))
        form_layout.addWidget(self.durum_combo)

        self.kaydet_btn = QPushButton("Kaydet")
        self.kaydet_btn.clicked.connect(self.kaydet)
        form_layout.addWidget(self.kaydet_btn)

        form_group.setLayout(form_layout)
        middle_layout.addWidget(form_group, 1)  # sol taraf küçük

        # SAĞ TABLO
        self.table = QTableWidget(0, 4)
        self.table.setHorizontalHeaderLabels(["ID", "Ad Soyad", "Telefon", "İl"])
        middle_layout.addWidget(self.table, 3)  # sağ taraf büyük

        main_layout.addLayout(middle_layout)
        self.setLayout(main_layout)

        self.counter = 1  # ID için

    def kaydet(self):
        ad = self.ad_entry.text()
        tel = self.tel_entry.text()
        il = self.il_combo.currentText()

        if not ad or not tel or il == "Seçiniz":
            return  # boş bırakılırsa ekleme

        row = self.table.rowCount()
        self.table.insertRow(row)
        self.table.setItem(row, 0, QTableWidgetItem(str(self.counter)))
        self.table.setItem(row, 1, QTableWidgetItem(ad))
        self.table.setItem(row, 2, QTableWidgetItem(tel))
        self.table.setItem(row, 3, QTableWidgetItem(il))

        self.counter += 1

        # Alanları temizle
        self.ad_entry.clear()
        self.tel_entry.clear()
        self.il_combo.setCurrentIndex(0)

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = ERP()
    window.show()
    sys.exit(app.exec())
