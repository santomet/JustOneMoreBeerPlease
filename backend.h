#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QDebug>
#include "tableinfo.h"
#include "beverageinfo.h"
#include "orderinfo.h"

class BackEnd : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(QString activeTable READ activeTable NOTIFY activeTableChanged)
    Q_PROPERTY(float costActiveTable READ activeTableCost NOTIFY costForActiveTableChanged)
    Q_PROPERTY(QVariant tables READ tables NOTIFY tablesChanged)
    Q_PROPERTY(QVariant beverages READ beverages NOTIFY beveragesChanged)
    Q_PROPERTY(QVariant orders READ ordersForActiveTable NOTIFY ordersChanged)


public:
    explicit BackEnd(QObject *parent = nullptr);

    QString userName();
    QVariant tables();
    QVariant beverages();
    QVariant ordersForActiveTable();
    QString activeTable();
    float activeTableCost();

    void setUserName(const QString &userName);


public slots:
    void setTables(QStringList tableNames);
    void setOrderWaitingTables(QStringList tableNames, bool waiting = false, bool revertOthers = false);
    void setActiveTable(const QString &tableName);

    void clearBeverages();
    void addBeverage(int id, QString name, QString size, float price);

    void makeOrder(); //basically takes active table and what is in mBeverages and resets mBeverages
    void fulfill();

signals:
    void userNameChanged();
    void tablesChanged();
    void beveragesChanged();
    void activeTableChanged();
    void costForActiveTableChanged();
    void ordersChanged();

private:
    QString m_userName;
    QString currency{"CZK"};
    QList<QObject*> mTables;
    QList<QObject*> mBeverages;

    QList<OrderInfo*> mOrders;
    float costForActiveTable{0};
    QString mActiveTable{""};

    bool demo{true};
};


#endif // BACKEND_H
