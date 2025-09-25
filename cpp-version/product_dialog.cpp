#include "product_dialog.h"
#include <QMessageBox>

ProductDialog::ProductDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUI();
    setupConnections();
}

ProductDialog::ProductDialog(const Product &product, QWidget *parent)
    : QDialog(parent)
    , m_product(product)
{
    setupUI();
    setupConnections();
    setProduct(product);
}

void ProductDialog::setupUI()
{
    setWindowTitle("Ürün Düzenle");
    setModal(true);
    setFixedSize(400, 200);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Form layout
    QGridLayout *formLayout = new QGridLayout();
    
    formLayout->addWidget(new QLabel("Ürün Adı:"), 0, 0);
    m_nameEdit = new QLineEdit();
    formLayout->addWidget(m_nameEdit, 0, 1);
    
    formLayout->addWidget(new QLabel("Ürün Kodu:"), 1, 0);
    m_codeEdit = new QLineEdit();
    formLayout->addWidget(m_codeEdit, 1, 1);
    
    formLayout->addWidget(new QLabel("Fiyat:"), 2, 0);
    m_priceSpinBox = new QDoubleSpinBox();
    m_priceSpinBox->setRange(0.0, 999999.99);
    m_priceSpinBox->setDecimals(2);
    m_priceSpinBox->setSuffix(" TL");
    formLayout->addWidget(m_priceSpinBox, 2, 1);
    
    formLayout->addWidget(new QLabel("Birim:"), 3, 0);
    m_unitCombo = new QComboBox();
    m_unitCombo->addItems({"Adet", "Kg", "Litre", "Paket", "Metre", "Gram"});
    formLayout->addWidget(m_unitCombo, 3, 1);
    
    mainLayout->addLayout(formLayout);
    
    // Button layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    m_okButton = new QPushButton("Tamam");
    m_okButton->setStyleSheet("QPushButton { background-color: white; border: 2px solid #999999; padding: 8px; }");
    buttonLayout->addWidget(m_okButton);
    
    m_cancelButton = new QPushButton("İptal");
    m_cancelButton->setStyleSheet("QPushButton { background-color: white; border: 2px solid #999999; padding: 8px; }");
    buttonLayout->addWidget(m_cancelButton);
    
    mainLayout->addLayout(buttonLayout);
    
    // Style
    setStyleSheet("QDialog { background-color: #d0d0d0; }");
}

void ProductDialog::setupConnections()
{
    connect(m_okButton, &QPushButton::clicked, this, &ProductDialog::onOkClicked);
    connect(m_cancelButton, &QPushButton::clicked, this, &ProductDialog::onCancelClicked);
}

Product ProductDialog::getProduct() const
{
    return m_product;
}

void ProductDialog::setProduct(const Product &product)
{
    m_product = product;
    m_nameEdit->setText(product.name());
    m_codeEdit->setText(product.code());
    m_priceSpinBox->setValue(product.price());
    
    int unitIndex = m_unitCombo->findText(product.unit());
    if (unitIndex >= 0) {
        m_unitCombo->setCurrentIndex(unitIndex);
    }
}

void ProductDialog::onOkClicked()
{
    QString name = m_nameEdit->text().trimmed();
    QString code = m_codeEdit->text().trimmed();
    double price = m_priceSpinBox->value();
    QString unit = m_unitCombo->currentText();
    
    if (name.isEmpty() || code.isEmpty()) {
        QMessageBox::warning(this, "Uyarı", "Lütfen ürün adı ve kodu girin!");
        return;
    }
    
    if (price <= 0) {
        QMessageBox::warning(this, "Uyarı", "Fiyat 0'dan büyük olmalıdır!");
        return;
    }
    
    m_product = Product(name, code, price, unit);
    accept();
}

void ProductDialog::onCancelClicked()
{
    reject();
}
