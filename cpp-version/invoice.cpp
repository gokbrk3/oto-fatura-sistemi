#include "invoice.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

InvoiceItem::InvoiceItem()
    : m_quantity(0)
{
}

InvoiceItem::InvoiceItem(const Product &product, int quantity)
    : m_product(product)
    , m_quantity(quantity)
{
}

QJsonObject InvoiceItem::toJson() const
{
    QJsonObject obj;
    obj["product"] = m_product.toJson();
    obj["quantity"] = m_quantity;
    return obj;
}

InvoiceItem InvoiceItem::fromJson(const QJsonObject &json)
{
    InvoiceItem item;
    item.m_product = Product::fromJson(json["product"].toObject());
    item.m_quantity = json["quantity"].toInt();
    return item;
}

Invoice::Invoice()
    : m_type(NORMAL)
    , m_status(DRAFT)
    , m_date(QDateTime::currentDateTime())
{
}

Invoice::Invoice(const QString &name, const QString &sube, const QString &personel, const QString &islem)
    : m_name(name)
    , m_sube(sube)
    , m_personel(personel)
    , m_islem(islem)
    , m_type(NORMAL)
    , m_status(DRAFT)
    , m_date(QDateTime::currentDateTime())
{
}

double Invoice::total() const
{
    double total = 0.0;
    for (const auto &value : m_items) {
        InvoiceItem item = InvoiceItem::fromJson(value.toObject());
        total += item.total();
    }
    return total;
}

void Invoice::addItem(const InvoiceItem &item)
{
    m_items.append(item.toJson());
}

void Invoice::removeItem(int index)
{
    if (index >= 0 && index < m_items.size()) {
        m_items.removeAt(index);
    }
}

void Invoice::clearItems()
{
    m_items.clear();
}

QJsonObject Invoice::toJson() const
{
    QJsonObject obj;
    obj["name"] = m_name;
    obj["sube"] = m_sube;
    obj["personel"] = m_personel;
    obj["islem"] = m_islem;
    obj["customer"] = m_customer.toJson();
    obj["items"] = m_items;
    obj["type"] = static_cast<int>(m_type);
    obj["status"] = static_cast<int>(m_status);
    obj["date"] = m_date.toString(Qt::ISODate);
    return obj;
}

Invoice Invoice::fromJson(const QJsonObject &json)
{
    Invoice invoice;
    invoice.m_name = json["name"].toString();
    invoice.m_sube = json["sube"].toString();
    invoice.m_personel = json["personel"].toString();
    invoice.m_islem = json["islem"].toString();
    invoice.m_customer = Customer::fromJson(json["customer"].toObject());
    invoice.m_items = json["items"].toArray();
    invoice.m_type = static_cast<InvoiceType>(json["type"].toInt());
    invoice.m_status = static_cast<InvoiceStatus>(json["status"].toInt());
    invoice.m_date = QDateTime::fromString(json["date"].toString(), Qt::ISODate);
    return invoice;
}

bool Invoice::isValid() const
{
    return !m_name.isEmpty() && !m_sube.isEmpty() && !m_personel.isEmpty() && !m_islem.isEmpty();
}
