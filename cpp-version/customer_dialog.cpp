#include "customer_dialog.h"
#include <QMessageBox>

CustomerDialog::CustomerDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUI();
    setupConnections();
}

CustomerDialog::CustomerDialog(const Customer &customer, QWidget *parent)
    : QDialog(parent)
    , m_customer(customer)
{
    setupUI();
    setupConnections();
    setCustomer(customer);
}

void CustomerDialog::setupUI()
{
    setWindowTitle("Müşteri Düzenle");
    setModal(true);
    setFixedSize(500, 300);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Form layout
    QGridLayout *formLayout = new QGridLayout();
    
    formLayout->addWidget(new QLabel("Müşteri Adı:"), 0, 0);
    m_nameEdit = new QLineEdit();
    formLayout->addWidget(m_nameEdit, 0, 1);
    
    formLayout->addWidget(new QLabel("Müşteri Kodu:"), 0, 2);
    m_codeEdit = new QLineEdit();
    formLayout->addWidget(m_codeEdit, 0, 3);
    
    formLayout->addWidget(new QLabel("Vergi No:"), 1, 0);
    m_taxEdit = new QLineEdit();
    formLayout->addWidget(m_taxEdit, 1, 1);
    
    formLayout->addWidget(new QLabel("Adres:"), 2, 0);
    m_addressEdit = new QTextEdit();
    m_addressEdit->setMaximumHeight(100);
    formLayout->addWidget(m_addressEdit, 2, 1, 1, 3);
    
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

void CustomerDialog::setupConnections()
{
    connect(m_okButton, &QPushButton::clicked, this, &CustomerDialog::onOkClicked);
    connect(m_cancelButton, &QPushButton::clicked, this, &CustomerDialog::onCancelClicked);
}

Customer CustomerDialog::getCustomer() const
{
    return m_customer;
}

void CustomerDialog::setCustomer(const Customer &customer)
{
    m_customer = customer;
    m_nameEdit->setText(customer.name());
    m_codeEdit->setText(customer.code());
    m_taxEdit->setText(customer.tax());
    m_addressEdit->setPlainText(customer.address());
}

void CustomerDialog::onOkClicked()
{
    QString name = m_nameEdit->text().trimmed();
    QString code = m_codeEdit->text().trimmed();
    QString tax = m_taxEdit->text().trimmed();
    QString address = m_addressEdit->toPlainText().trimmed();
    
    if (name.isEmpty() || code.isEmpty()) {
        QMessageBox::warning(this, "Uyarı", "Lütfen müşteri adı ve kodu girin!");
        return;
    }
    
    m_customer = Customer(name, code, tax, address);
    accept();
}

void CustomerDialog::onCancelClicked()
{
    reject();
}
