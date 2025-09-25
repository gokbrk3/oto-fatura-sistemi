#include "customer.h"
#include <QJsonObject>

Customer::Customer()
{
}

Customer::Customer(const QString &name, const QString &code, const QString &tax, const QString &address)
    : m_name(name)
    , m_code(code)
    , m_tax(tax)
    , m_address(address)
{
}

QJsonObject Customer::toJson() const
{
    QJsonObject obj;
    obj["name"] = m_name;
    obj["code"] = m_code;
    obj["tax"] = m_tax;
    obj["address"] = m_address;
    return obj;
}

Customer Customer::fromJson(const QJsonObject &json)
{
    Customer customer;
    customer.m_name = json["name"].toString();
    customer.m_code = json["code"].toString();
    customer.m_tax = json["tax"].toString();
    customer.m_address = json["address"].toString();
    return customer;
}

bool Customer::isValid() const
{
    return !m_name.isEmpty() && !m_code.isEmpty();
}
