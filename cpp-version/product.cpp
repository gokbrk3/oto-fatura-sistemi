#include "product.h"
#include <QJsonObject>

Product::Product()
    : m_price(0.0)
{
}

Product::Product(const QString &name, const QString &code, double price, const QString &unit)
    : m_name(name)
    , m_code(code)
    , m_price(price)
    , m_unit(unit)
{
}

QJsonObject Product::toJson() const
{
    QJsonObject obj;
    obj["name"] = m_name;
    obj["code"] = m_code;
    obj["price"] = m_price;
    obj["unit"] = m_unit;
    return obj;
}

Product Product::fromJson(const QJsonObject &json)
{
    Product product;
    product.m_name = json["name"].toString();
    product.m_code = json["code"].toString();
    product.m_price = json["price"].toDouble();
    product.m_unit = json["unit"].toString();
    return product;
}

bool Product::isValid() const
{
    return !m_name.isEmpty() && !m_code.isEmpty() && m_price > 0;
}
