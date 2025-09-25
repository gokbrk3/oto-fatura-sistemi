#ifndef PRODUCT_DIALOG_H
#define PRODUCT_DIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QDoubleSpinBox>
#include "product.h"

class ProductDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProductDialog(QWidget *parent = nullptr);
    explicit ProductDialog(const Product &product, QWidget *parent = nullptr);
    
    Product getProduct() const;
    void setProduct(const Product &product);
    
private slots:
    void onOkClicked();
    void onCancelClicked();
    
private:
    void setupUI();
    void setupConnections();
    
    // UI Components
    QLineEdit *m_nameEdit;
    QLineEdit *m_codeEdit;
    QDoubleSpinBox *m_priceSpinBox;
    QComboBox *m_unitCombo;
    QPushButton *m_okButton;
    QPushButton *m_cancelButton;
    
    Product m_product;
};

#endif // PRODUCT_DIALOG_H
