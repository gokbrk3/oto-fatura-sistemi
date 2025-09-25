#ifndef SELENIUM_WRAPPER_H
#define SELENIUM_WRAPPER_H

#include <QString>
#include <QJsonArray>
#include <QObject>
#include <QThread>

class SeleniumWrapper : public QObject
{
    Q_OBJECT

public:
    explicit SeleniumWrapper(QObject *parent = nullptr);
    ~SeleniumWrapper();
    
    // Configuration
    void setCredentials(const QString &username, const QString &password);
    void setHeadless(bool headless);
    
    // Draft invoice operations
    void readDraftInvoices();
    
signals:
    void draftInvoicesRead(const QJsonArray &invoices);
    void errorOccurred(const QString &error);
    void progressUpdated(int percentage);
    
private:
    QString m_username;
    QString m_password;
    bool m_headless;
    
    // Internal methods
    bool login();
    bool navigateToDrafts();
    QJsonArray readEFaturaDrafts();
    QJsonArray readEArsivDrafts();
    void cleanup();
};

class SeleniumWorker : public QThread
{
    Q_OBJECT

public:
    explicit SeleniumWorker(QObject *parent = nullptr);
    void setWrapper(SeleniumWrapper *wrapper);
    
protected:
    void run() override;
    
signals:
    void draftInvoicesRead(const QJsonArray &invoices);
    void errorOccurred(const QString &error);
    void progressUpdated(int percentage);
    
private:
    SeleniumWrapper *m_wrapper;
};

#endif // SELENIUM_WRAPPER_H
