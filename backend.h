#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QDebug>
#include "apiclient.h"
#include "tableinfo.h"
#include "beverageinfo.h"
#include "orderinfo.h"

class BackEnd : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(QString activeTable READ activeTable NOTIFY activeTableChanged)
    Q_PROPERTY(int activeTableId READ activeTableId WRITE setActiveTableId NOTIFY activeTableIdChanged)
    Q_PROPERTY(float costActiveTable READ activeTableCost NOTIFY costForActiveTableChanged)
    Q_PROPERTY(QVariant tables READ tables NOTIFY tablesChanged)
    Q_PROPERTY(QVariant beverages READ beverages NOTIFY beveragesChanged)
    Q_PROPERTY(QVariant orders READ ordersForActiveTable NOTIFY ordersChanged)


public:
    explicit BackEnd(QObject *parent = nullptr);
    ~BackEnd();

    QString userName();
    QVariant tables();
    QVariant beverages();
    QVariant ordersForActiveTable();
    QString activeTable();
    int activeTableId();
    float activeTableCost();

    void setUserName(const QString &userName);


public slots:
    void setTables(QList<int> ids, QStringList names, QList<bool> waitingOrders);
    void setOrderWaitingTables(QStringList tableNames, bool waiting = false, bool revertOthers = false);
    void setActiveTable(const QString &tableName);
    void setActiveTableId(const int id);

    void clearBeverages();
    void setBeverages(QList<int> id, QStringList names, QStringList sizes, QList<double> prices);
    void addBeverage(int id, QString name, QString size, float price);

    void setOrders(QList<QPair<int,QList<int>>> orders); //for active table
    void makeOrder(); //basically takes active table and what is in mBeverages and resets mBeverages
    void fulfill();

    //for apiclient
    void requestTables();
    void requestOrdersForActiveTable();
    void requestMenu();

signals:
    void userNameChanged();
    void tablesChanged();
    void beveragesChanged();
    void activeTableChanged();
    void activeTableIdChanged();
    void costForActiveTableChanged();
    void ordersChanged();

    //for api client
    void requestTablesSignal();
    void requestOrdersSignal(int tableId);
    void requestMenuSignal();
    void requestFulfillOrdersSignal(QList<QPair<int, QPair<bool,QList<int> > > > orders);
    void requestCreateOrder(int tableId, QList<int> bevIds);

private:
    QString m_userName;
    QString currency{"CZK"};
    QList<QObject*> mTables;
    QList<QObject*> mBeverages;

    QList<OrderInfo*> mOrders;
    float costForActiveTable{0};
    QString mActiveTable{""};
    int mActiveTableId{0};

    bool demo{false};
    ApiClient *client{nullptr};
};


#endif // BACKEND_H
