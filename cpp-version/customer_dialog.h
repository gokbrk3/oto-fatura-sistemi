#ifndef CUSTOMER_DIALOG_H
#define CUSTOMER_DIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QTextEdit>
#include "customer.h"

class CustomerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CustomerDialog(QWidget *parent = nullptr);
    explicit CustomerDialog(const Customer &customer, QWidget *parent = nullptr);
    
    Customer getCustomer() const;
    void setCustomer(const Customer &customer);
    
private slots:
    void onOkClicked();
    void onCancelClicked();
    
private:
    void setupUI();
    void setupConnections();
    
    // UI Components
    QLineEdit *m_nameEdit;
    QLineEdit *m_codeEdit;
    QLineEdit *m_taxEdit;
    QTextEdit *m_addressEdit;
    QPushButton *m_okButton;
    QPushButton *m_cancelButton;
    
    Customer m_customer;
};

#endif // CUSTOMER_DIALOG_H
