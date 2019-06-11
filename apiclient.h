#ifndef APICLIENT_H
#define APICLIENT_H

#include <QObject>
#include <QtNetwork>
#include <QThread>

class ApiClient : public QObject
{
    Q_OBJECT
public:
    explicit ApiClient(QObject *parent = nullptr);
    ~ApiClient();

signals:
    void tablesResponse(QList<int> ids, QStringList names, QList<bool> waiting);
    void menuResponse(QList<int> ids, QStringList names, QStringList sizes, QList<double> prices);
    void ordersResponse(QList<QPair<int,QList<int>>> ordersForTable);

    void fulfillOrdersFinished();
    void createOrderFinished();

public slots:
    void init();
    void destroy();

    void requestMenu();
    void requestTables();
    void requestOrders(int tableId);

    void fulfillOrders(QList<QPair<int, QPair<bool, QList<int> > > > orders);
    void createOrder(int tableId, QList<int> menuIds);


private:
    QThread *mThread{nullptr};
    QNetworkAccessManager *nam;
    QString urlbase{"http://barordersystem.eu-gb.mybluemix.net/"};

};

#endif // APICLIENT_H
