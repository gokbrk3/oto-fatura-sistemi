#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <QString>
#include <QJsonObject>

class Customer
{
public:
    Customer();
    Customer(const QString &name, const QString &code, const QString &tax, const QString &address);
    
    // Getters
    QString name() const { return m_name; }
    QString code() const { return m_code; }
    QString tax() const { return m_tax; }
    QString address() const { return m_address; }
    
    // Setters
    void setName(const QString &name) { m_name = name; }
    void setCode(const QString &code) { m_code = code; }
    void setTax(const QString &tax) { m_tax = tax; }
    void setAddress(const QString &address) { m_address = address; }
    
    // JSON serialization
    QJsonObject toJson() const;
    static Customer fromJson(const QJsonObject &json);
    
    // Validation
    bool isValid() const;
    
private:
    QString m_name;
    QString m_code;
    QString m_tax;
    QString m_address;
};

#endif // CUSTOMER_H
