#ifndef PRODUCT_H
#define PRODUCT_H

#include <QString>
#include <QJsonObject>

class Product
{
public:
    Product();
    Product(const QString &name, const QString &code, double price, const QString &unit);
    
    // Getters
    QString name() const { return m_name; }
    QString code() const { return m_code; }
    double price() const { return m_price; }
    QString unit() const { return m_unit; }
    
    // Setters
    void setName(const QString &name) { m_name = name; }
    void setCode(const QString &code) { m_code = code; }
    void setPrice(double price) { m_price = price; }
    void setUnit(const QString &unit) { m_unit = unit; }
    
    // JSON serialization
    QJsonObject toJson() const;
    static Product fromJson(const QJsonObject &json);
    
    // Validation
    bool isValid() const;
    
private:
    QString m_name;
    QString m_code;
    double m_price;
    QString m_unit;
};

#endif // PRODUCT_H
