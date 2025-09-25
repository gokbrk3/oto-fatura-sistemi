#include <QApplication>
#include <QMainWindow>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QGroupBox>
#include <QLabel>
#include <QTextEdit>
#include <QCheckBox>
#include <QMessageBox>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStyleFactory>
#include <QApplication>
#include <QThread>
#include <QProgressDialog>
#include "product.h"
#include "customer.h"
#include "invoice.h"
#include "selenium_wrapper.h"
#include "product_dialog.h"
#include "customer_dialog.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onProductAdd();
    void onProductEdit();
    void onProductDelete();
    void onCustomerAdd();
    void onCustomerEdit();
    void onCustomerDelete();
    void onInvoiceCreate();
    void onInvoiceProcess();
    void onDraftRead();
    void onProductSelectionChanged();
    void onCustomerSelectionChanged();
    void onInvoiceSelectionChanged();
    void loadData();
    void saveData();

private:
    void setupUI();
    void setupProductTab();
    void setupCustomerTab();
    void setupInvoiceDraftTab();
    void setupInvoiceProcessTab();
    void applyZebraStriping(QTableWidget *table);
    
    // UI Components
    QTabWidget *m_tabWidget;
    
    // Product Tab
    QTableWidget *m_productTable;
    QLineEdit *m_productNameEdit;
    QLineEdit *m_productCodeEdit;
    QLineEdit *m_productPriceEdit;
    QComboBox *m_productUnitCombo;
    
    // Customer Tab
    QTableWidget *m_customerTable;
    QLineEdit *m_customerNameEdit;
    QLineEdit *m_customerCodeEdit;
    QLineEdit *m_customerTaxEdit;
    QLineEdit *m_customerAddressEdit;
    
    // Invoice Draft Tab
    QTableWidget *m_invoiceTable;
    QComboBox *m_subeCombo;
    QComboBox *m_personelCombo;
    QComboBox *m_islemCombo;
    QLineEdit *m_invoiceNameEdit;
    QCheckBox *m_headlessCheck;
    
    // Invoice Process Tab
    QTableWidget *m_draftTable;
    QComboBox *m_draftSubeCombo;
    QComboBox *m_draftPersonelCombo;
    QComboBox *m_draftIslemCombo;
    
    // Data
    QJsonArray m_products;
    QJsonArray m_customers;
    QJsonArray m_invoices;
    QJsonArray m_drafts;
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Oto Fatura Sistemi - C++");
    setMinimumSize(1200, 800);
    
    setupUI();
    loadData();
}

MainWindow::~MainWindow()
{
    saveData();
}

void MainWindow::setupUI()
{
    m_tabWidget = new QTabWidget(this);
    setCentralWidget(m_tabWidget);
    
    setupProductTab();
    setupCustomerTab();
    setupInvoiceDraftTab();
    setupInvoiceProcessTab();
    
    // Style
    setStyleSheet("QMainWindow { background-color: #d0d0d0; }");
    m_tabWidget->setStyleSheet(
        "QTabWidget::pane { border: 2px solid #c0c0c0; background-color: #d0d0d0; }"
        "QTabBar::tab { background-color: #d0d0d0; border: 1px solid #999999; padding: 8px; }"
        "QTabBar::tab:selected { background-color: white; }"
        "QTabBar::tab:hover { background-color: #e0e0e0; }"
    );
}

void MainWindow::setupProductTab()
{
    QWidget *productWidget = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(productWidget);
    
    // Product Form
    QGroupBox *formGroup = new QGroupBox("Kart Ekleme Alanı");
    formGroup->setStyleSheet("QGroupBox { background-color: #d0d0d0; border: 2px solid #999999; padding: 10px; }");
    
    QGridLayout *formLayout = new QGridLayout(formGroup);
    
    formLayout->addWidget(new QLabel("Ürün Adı:"), 0, 0);
    m_productNameEdit = new QLineEdit();
    formLayout->addWidget(m_productNameEdit, 0, 1);
    
    formLayout->addWidget(new QLabel("Ürün Kodu:"), 0, 2);
    m_productCodeEdit = new QLineEdit();
    formLayout->addWidget(m_productCodeEdit, 0, 3);
    
    formLayout->addWidget(new QLabel("Fiyat:"), 1, 0);
    m_productPriceEdit = new QLineEdit();
    formLayout->addWidget(m_productPriceEdit, 1, 1);
    
    formLayout->addWidget(new QLabel("Birim:"), 1, 2);
    m_productUnitCombo = new QComboBox();
    m_productUnitCombo->addItems({"Adet", "Kg", "Litre", "Paket"});
    formLayout->addWidget(m_productUnitCombo, 1, 3);
    
    QPushButton *addButton = new QPushButton("Ürün Ekle");
    addButton->setStyleSheet("QPushButton { background-color: white; border: 2px solid #999999; padding: 8px; }");
    connect(addButton, &QPushButton::clicked, this, &MainWindow::onProductAdd);
    formLayout->addWidget(addButton, 2, 0, 1, 2);
    
    QPushButton *editButton = new QPushButton("Düzenle");
    editButton->setStyleSheet("QPushButton { background-color: white; border: 2px solid #999999; padding: 8px; }");
    connect(editButton, &QPushButton::clicked, this, &MainWindow::onProductEdit);
    formLayout->addWidget(editButton, 2, 2, 1, 2);
    
    mainLayout->addWidget(formGroup);
    
    // Product Table
    QGroupBox *tableGroup = new QGroupBox("Ürün Kartları");
    tableGroup->setStyleSheet("QGroupBox { background-color: #d0d0d0; border: 2px solid #999999; padding: 10px; }");
    
    QVBoxLayout *tableLayout = new QVBoxLayout(tableGroup);
    
    m_productTable = new QTableWidget();
    m_productTable->setColumnCount(5);
    m_productTable->setHorizontalHeaderLabels({"Ürün Adı", "Ürün Kodu", "Fiyat", "Birim", "İşlemler"});
    m_productTable->horizontalHeader()->setStretchLastSection(true);
    
    tableLayout->addWidget(m_productTable);
    mainLayout->addWidget(tableGroup);
    
    m_tabWidget->addTab(productWidget, "Ürün Kartları");
}

void MainWindow::setupCustomerTab()
{
    QWidget *customerWidget = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(customerWidget);
    
    // Customer Form
    QGroupBox *formGroup = new QGroupBox("Müşteri Ekleme Alanı");
    formGroup->setStyleSheet("QGroupBox { background-color: #d0d0d0; border: 2px solid #999999; padding: 10px; }");
    
    QGridLayout *formLayout = new QGridLayout(formGroup);
    
    formLayout->addWidget(new QLabel("Müşteri Adı:"), 0, 0);
    m_customerNameEdit = new QLineEdit();
    formLayout->addWidget(m_customerNameEdit, 0, 1);
    
    formLayout->addWidget(new QLabel("Müşteri Kodu:"), 0, 2);
    m_customerCodeEdit = new QLineEdit();
    formLayout->addWidget(m_customerCodeEdit, 0, 3);
    
    formLayout->addWidget(new QLabel("Vergi No:"), 1, 0);
    m_customerTaxEdit = new QLineEdit();
    formLayout->addWidget(m_customerTaxEdit, 1, 1);
    
    formLayout->addWidget(new QLabel("Adres:"), 1, 2);
    m_customerAddressEdit = new QLineEdit();
    formLayout->addWidget(m_customerAddressEdit, 1, 3);
    
    QPushButton *addButton = new QPushButton("Müşteri Ekle");
    addButton->setStyleSheet("QPushButton { background-color: white; border: 2px solid #999999; padding: 8px; }");
    connect(addButton, &QPushButton::clicked, this, &MainWindow::onCustomerAdd);
    formLayout->addWidget(addButton, 2, 0, 1, 2);
    
    mainLayout->addWidget(formGroup);
    
    // Customer Table
    QGroupBox *tableGroup = new QGroupBox("Müşteriler");
    tableGroup->setStyleSheet("QGroupBox { background-color: #d0d0d0; border: 2px solid #999999; padding: 10px; }");
    
    QVBoxLayout *tableLayout = new QVBoxLayout(tableGroup);
    
    m_customerTable = new QTableWidget();
    m_customerTable->setColumnCount(5);
    m_customerTable->setHorizontalHeaderLabels({"Müşteri Adı", "Müşteri Kodu", "Vergi No", "Adres", "İşlemler"});
    m_customerTable->horizontalHeader()->setStretchLastSection(true);
    
    tableLayout->addWidget(m_customerTable);
    mainLayout->addWidget(tableGroup);
    
    m_tabWidget->addTab(customerWidget, "Müşteriler");
}

void MainWindow::setupInvoiceDraftTab()
{
    QWidget *invoiceWidget = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(invoiceWidget);
    
    // Invoice Form
    QGroupBox *formGroup = new QGroupBox("Fatura Oluşturma");
    formGroup->setStyleSheet("QGroupBox { background-color: #d0d0d0; border: 2px solid #999999; padding: 10px; }");
    
    QGridLayout *formLayout = new QGridLayout(formGroup);
    
    formLayout->addWidget(new QLabel("Şube:"), 0, 0);
    m_subeCombo = new QComboBox();
    m_subeCombo->addItems({"Şube 1", "Şube 2", "Şube 3"});
    formLayout->addWidget(m_subeCombo, 0, 1);
    
    formLayout->addWidget(new QLabel("Personel:"), 0, 2);
    m_personelCombo = new QComboBox();
    m_personelCombo->addItems({"Personel 1", "Personel 2", "Personel 3"});
    formLayout->addWidget(m_personelCombo, 0, 3);
    
    formLayout->addWidget(new QLabel("İşlem Türü:"), 1, 0);
    m_islemCombo = new QComboBox();
    m_islemCombo->addItems({"Satış", "Alış", "İade"});
    formLayout->addWidget(m_islemCombo, 1, 1);
    
    formLayout->addWidget(new QLabel("Fatura Adı:"), 1, 2);
    m_invoiceNameEdit = new QLineEdit();
    formLayout->addWidget(m_invoiceNameEdit, 1, 3);
    
    m_headlessCheck = new QCheckBox("Headless Mode");
    m_headlessCheck->setStyleSheet("QCheckBox { background-color: #d0d0d0; }");
    formLayout->addWidget(m_headlessCheck, 2, 0, 1, 2);
    
    QPushButton *createButton = new QPushButton("Fatura Oluştur");
    createButton->setStyleSheet("QPushButton { background-color: white; border: 2px solid #999999; padding: 8px; }");
    connect(createButton, &QPushButton::clicked, this, &MainWindow::onInvoiceCreate);
    formLayout->addWidget(createButton, 2, 2, 1, 2);
    
    mainLayout->addWidget(formGroup);
    
    // Invoice Table
    QGroupBox *tableGroup = new QGroupBox("Fatura Ürünleri");
    tableGroup->setStyleSheet("QGroupBox { background-color: #d0d0d0; border: 2px solid #999999; padding: 10px; }");
    
    QVBoxLayout *tableLayout = new QVBoxLayout(tableGroup);
    
    m_invoiceTable = new QTableWidget();
    m_invoiceTable->setColumnCount(6);
    m_invoiceTable->setHorizontalHeaderLabels({"Ürün", "Miktar", "Fiyat", "Toplam", "İşlemler"});
    m_invoiceTable->horizontalHeader()->setStretchLastSection(true);
    
    tableLayout->addWidget(m_invoiceTable);
    mainLayout->addWidget(tableGroup);
    
    m_tabWidget->addTab(invoiceWidget, "Fatura Taslak Oluştur");
}

void MainWindow::setupInvoiceProcessTab()
{
    QWidget *processWidget = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(processWidget);
    
    // Draft Selection
    QGroupBox *selectionGroup = new QGroupBox("Fatura İsimlendirme");
    selectionGroup->setStyleSheet("QGroupBox { background-color: #d0d0d0; border: 2px solid #999999; padding: 10px; }");
    
    QGridLayout *selectionLayout = new QGridLayout(selectionGroup);
    
    selectionLayout->addWidget(new QLabel("Şube:"), 0, 0);
    m_draftSubeCombo = new QComboBox();
    m_draftSubeCombo->addItems({"Şube 1", "Şube 2", "Şube 3"});
    selectionLayout->addWidget(m_draftSubeCombo, 0, 1);
    
    selectionLayout->addWidget(new QLabel("Personel:"), 0, 2);
    m_draftPersonelCombo = new QComboBox();
    m_draftPersonelCombo->addItems({"Personel 1", "Personel 2", "Personel 3"});
    selectionLayout->addWidget(m_draftPersonelCombo, 0, 3);
    
    selectionLayout->addWidget(new QLabel("İşlem Türü:"), 1, 0);
    m_draftIslemCombo = new QComboBox();
    m_draftIslemCombo->addItems({"Satış", "Alış", "İade"});
    selectionLayout->addWidget(m_draftIslemCombo, 1, 1);
    
    QPushButton *readButton = new QPushButton("Taslak Faturaları Oku");
    readButton->setStyleSheet("QPushButton { background-color: white; border: 2px solid #999999; padding: 8px; }");
    connect(readButton, &QPushButton::clicked, this, &MainWindow::onDraftRead);
    selectionLayout->addWidget(readButton, 1, 2, 1, 2);
    
    mainLayout->addWidget(selectionGroup);
    
    // Draft Table
    QGroupBox *tableGroup = new QGroupBox("Fatura Listesi");
    tableGroup->setStyleSheet("QGroupBox { background-color: #d0d0d0; border: 2px solid #999999; padding: 10px; }");
    
    QVBoxLayout *tableLayout = new QVBoxLayout(tableGroup);
    
    m_draftTable = new QTableWidget();
    m_draftTable->setColumnCount(7);
    m_draftTable->setHorizontalHeaderLabels({"Müşteri", "Vergi No", "Tutar", "Durum", "Tarih", "Fatura Türü", "İşlemler"});
    m_draftTable->horizontalHeader()->setStretchLastSection(true);
    
    tableLayout->addWidget(m_draftTable);
    mainLayout->addWidget(tableGroup);
    
    m_tabWidget->addTab(processWidget, "Fatura Kes");
}

void MainWindow::onProductAdd()
{
    QString name = m_productNameEdit->text();
    QString code = m_productCodeEdit->text();
    QString price = m_productPriceEdit->text();
    QString unit = m_productUnitCombo->currentText();
    
    if (name.isEmpty() || code.isEmpty() || price.isEmpty()) {
        QMessageBox::warning(this, "Uyarı", "Lütfen tüm alanları doldurun!");
        return;
    }
    
    QJsonObject product;
    product["name"] = name;
    product["code"] = code;
    product["price"] = price.toDouble();
    product["unit"] = unit;
    
    m_products.append(product);
    
    // Clear form
    m_productNameEdit->clear();
    m_productCodeEdit->clear();
    m_productPriceEdit->clear();
    
    // Refresh table
    loadData();
}

void MainWindow::onProductEdit()
{
    int row = m_productTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Uyarı", "Lütfen düzenlenecek ürünü seçin!");
        return;
    }
    
    // Get current product data
    QJsonObject productJson = m_products[row].toObject();
    Product product = Product::fromJson(productJson);
    
    // Open edit dialog
    ProductDialog dialog(product, this);
    if (dialog.exec() == QDialog::Accepted) {
        Product updatedProduct = dialog.getProduct();
        m_products[row] = updatedProduct.toJson();
        loadData();
    }
}

void MainWindow::onCustomerAdd()
{
    QString name = m_customerNameEdit->text();
    QString code = m_customerCodeEdit->text();
    QString tax = m_customerTaxEdit->text();
    QString address = m_customerAddressEdit->text();
    
    if (name.isEmpty() || code.isEmpty()) {
        QMessageBox::warning(this, "Uyarı", "Lütfen müşteri adı ve kodu girin!");
        return;
    }
    
    QJsonObject customer;
    customer["name"] = name;
    customer["code"] = code;
    customer["tax"] = tax;
    customer["address"] = address;
    
    m_customers.append(customer);
    
    // Clear form
    m_customerNameEdit->clear();
    m_customerCodeEdit->clear();
    m_customerTaxEdit->clear();
    m_customerAddressEdit->clear();
    
    // Refresh table
    loadData();
}

void MainWindow::onInvoiceCreate()
{
    QString sube = m_subeCombo->currentText();
    QString personel = m_personelCombo->currentText();
    QString islem = m_islemCombo->currentText();
    QString name = m_invoiceNameEdit->text();
    
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Uyarı", "Lütfen fatura adını girin!");
        return;
    }
    
    QMessageBox::information(this, "Bilgi", QString("Fatura oluşturuluyor:\nŞube: %1\nPersonel: %2\nİşlem: %3\nAd: %4")
                             .arg(sube).arg(personel).arg(islem).arg(name));
}

void MainWindow::onDraftRead()
{
    // Create progress dialog
    QProgressDialog progress("Taslak faturalar okunuyor...", "İptal", 0, 100, this);
    progress.setWindowModality(Qt::WindowModal);
    progress.show();
    
    // Create Selenium wrapper
    SeleniumWrapper *wrapper = new SeleniumWrapper(this);
    wrapper->setCredentials("zirve_user", "zirve_pass"); // TODO: Get from settings
    wrapper->setHeadless(m_headlessCheck->isChecked());
    
    // Create worker thread
    SeleniumWorker *worker = new SeleniumWorker(this);
    worker->setWrapper(wrapper);
    
    // Connect signals
    connect(worker, &SeleniumWorker::draftInvoicesRead, this, [this](const QJsonArray &invoices) {
        m_drafts = invoices;
        loadData();
        QMessageBox::information(this, "Başarılı", QString("%1 taslak fatura okundu!").arg(invoices.size()));
    });
    
    connect(worker, &SeleniumWorker::errorOccurred, this, [this](const QString &error) {
        QMessageBox::critical(this, "Hata", error);
    });
    
    connect(worker, &SeleniumWorker::progressUpdated, &progress, &QProgressDialog::setValue);
    
    connect(worker, &SeleniumWorker::finished, worker, &QObject::deleteLater);
    connect(worker, &SeleniumWorker::finished, wrapper, &QObject::deleteLater);
    
    // Start worker
    worker->start();
}

void MainWindow::loadData()
{
    // Load Products
    m_productTable->setRowCount(m_products.size());
    for (int i = 0; i < m_products.size(); ++i) {
        QJsonObject product = m_products[i].toObject();
        m_productTable->setItem(i, 0, new QTableWidgetItem(product["name"].toString()));
        m_productTable->setItem(i, 1, new QTableWidgetItem(product["code"].toString()));
        m_productTable->setItem(i, 2, new QTableWidgetItem(QString::number(product["price"].toDouble())));
        m_productTable->setItem(i, 3, new QTableWidgetItem(product["unit"].toString()));
        
        QPushButton *deleteBtn = new QPushButton("Sil");
        deleteBtn->setStyleSheet("QPushButton { background-color: white; border: 1px solid #999999; padding: 4px; }");
        m_productTable->setCellWidget(i, 4, deleteBtn);
        connect(deleteBtn, &QPushButton::clicked, [this, i]() {
            m_products.removeAt(i);
            loadData();
        });
    }
    applyZebraStriping(m_productTable);
    
    // Load Customers
    m_customerTable->setRowCount(m_customers.size());
    for (int i = 0; i < m_customers.size(); ++i) {
        QJsonObject customer = m_customers[i].toObject();
        m_customerTable->setItem(i, 0, new QTableWidgetItem(customer["name"].toString()));
        m_customerTable->setItem(i, 1, new QTableWidgetItem(customer["code"].toString()));
        m_customerTable->setItem(i, 2, new QTableWidgetItem(customer["tax"].toString()));
        m_customerTable->setItem(i, 3, new QTableWidgetItem(customer["address"].toString()));
        
        QPushButton *deleteBtn = new QPushButton("Sil");
        deleteBtn->setStyleSheet("QPushButton { background-color: white; border: 1px solid #999999; padding: 4px; }");
        m_customerTable->setCellWidget(i, 4, deleteBtn);
        connect(deleteBtn, &QPushButton::clicked, [this, i]() {
            m_customers.removeAt(i);
            loadData();
        });
    }
    applyZebraStriping(m_customerTable);
    
    // Load Draft Invoices
    m_draftTable->setRowCount(m_drafts.size());
    for (int i = 0; i < m_drafts.size(); ++i) {
        QJsonObject draft = m_drafts[i].toObject();
        m_draftTable->setItem(i, 0, new QTableWidgetItem(draft["customer"].toString()));
        m_draftTable->setItem(i, 1, new QTableWidgetItem(draft["tax"].toString()));
        m_draftTable->setItem(i, 2, new QTableWidgetItem(draft["amount"].toString()));
        m_draftTable->setItem(i, 3, new QTableWidgetItem(draft["status"].toString()));
        m_draftTable->setItem(i, 4, new QTableWidgetItem(draft["date"].toString()));
        m_draftTable->setItem(i, 5, new QTableWidgetItem(draft["type"].toString()));
        
        QPushButton *processBtn = new QPushButton("İşle");
        processBtn->setStyleSheet("QPushButton { background-color: white; border: 1px solid #999999; padding: 4px; }");
        m_draftTable->setCellWidget(i, 6, processBtn);
        connect(processBtn, &QPushButton::clicked, [this, i]() {
            QMessageBox::information(this, "Bilgi", QString("Fatura %1 işleniyor...").arg(i+1));
        });
    }
    applyZebraStriping(m_draftTable);
}

void MainWindow::saveData()
{
    QJsonObject data;
    data["products"] = m_products;
    data["customers"] = m_customers;
    data["invoices"] = m_invoices;
    data["drafts"] = m_drafts;
    
    QJsonDocument doc(data);
    QFile file("data.json");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
    }
}

void MainWindow::applyZebraStriping(QTableWidget *table)
{
    for (int i = 0; i < table->rowCount(); ++i) {
        if (i % 2 == 0) {
            for (int j = 0; j < table->columnCount(); ++j) {
                QTableWidgetItem *item = table->item(i, j);
                if (item) {
                    item->setBackground(QColor("#f0f0f0"));
                }
            }
        } else {
            for (int j = 0; j < table->columnCount(); ++j) {
                QTableWidgetItem *item = table->item(i, j);
                if (item) {
                    item->setBackground(QColor("#e0e0e0"));
                }
            }
        }
    }
}

// Empty implementations for unused slots
void MainWindow::onProductDelete() {}
void MainWindow::onCustomerEdit() {}
void MainWindow::onCustomerDelete() {}
void MainWindow::onInvoiceProcess() {}
void MainWindow::onProductSelectionChanged() {}
void MainWindow::onCustomerSelectionChanged() {}
void MainWindow::onInvoiceSelectionChanged() {}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    MainWindow window;
    window.show();
    
    return app.exec();
}

#include "main.moc"
