#ifndef INVOICE_H
#define INVOICE_H

#include <QString>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonArray>
#include "product.h"
#include "customer.h"

class InvoiceItem
{
public:
    InvoiceItem();
    InvoiceItem(const Product &product, int quantity);
    
    // Getters
    Product product() const { return m_product; }
    int quantity() const { return m_quantity; }
    double total() const { return m_product.price() * m_quantity; }
    
    // Setters
    void setProduct(const Product &product) { m_product = product; }
    void setQuantity(int quantity) { m_quantity = quantity; }
    
    // JSON serialization
    QJsonObject toJson() const;
    static InvoiceItem fromJson(const QJsonObject &json);
    
private:
    Product m_product;
    int m_quantity;
};

class Invoice
{
public:
    enum InvoiceType {
        E_FATURA,
        E_ARSIV,
        NORMAL
    };
    
    enum InvoiceStatus {
        DRAFT,
        SENT,
        PROCESSED,
        CANCELLED
    };
    
    Invoice();
    Invoice(const QString &name, const QString &sube, const QString &personel, const QString &islem);
    
    // Getters
    QString name() const { return m_name; }
    QString sube() const { return m_sube; }
    QString personel() const { return m_personel; }
    QString islem() const { return m_islem; }
    Customer customer() const { return m_customer; }
    QJsonArray items() const { return m_items; }
    InvoiceType type() const { return m_type; }
    InvoiceStatus status() const { return m_status; }
    QDateTime date() const { return m_date; }
    double total() const;
    
    // Setters
    void setName(const QString &name) { m_name = name; }
    void setSube(const QString &sube) { m_sube = sube; }
    void setPersonel(const QString &personel) { m_personel = personel; }
    void setIslem(const QString &islem) { m_islem = islem; }
    void setCustomer(const Customer &customer) { m_customer = customer; }
    void setType(InvoiceType type) { m_type = type; }
    void setStatus(InvoiceStatus status) { m_status = status; }
    
    // Items management
    void addItem(const InvoiceItem &item);
    void removeItem(int index);
    void clearItems();
    
    // JSON serialization
    QJsonObject toJson() const;
    static Invoice fromJson(const QJsonObject &json);
    
    // Validation
    bool isValid() const;
    
private:
    QString m_name;
    QString m_sube;
    QString m_personel;
    QString m_islem;
    Customer m_customer;
    QJsonArray m_items;
    InvoiceType m_type;
    InvoiceStatus m_status;
    QDateTime m_date;
};

#endif // INVOICE_H
