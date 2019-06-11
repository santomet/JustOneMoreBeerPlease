#include "apiclient.h"

ApiClient::ApiClient(QObject *parent) : QObject(parent)
{
    mThread = new QThread();
    this->moveToThread(mThread);
    mThread->start();
    QMetaObject::invokeMethod(this, &ApiClient::init, Qt::BlockingQueuedConnection);
}

ApiClient::~ApiClient()
{
    QMetaObject::invokeMethod(this, &ApiClient::destroy, Qt::BlockingQueuedConnection);
    mThread->exit();
}

void ApiClient::init()
{
    nam = new QNetworkAccessManager();
}

void ApiClient::destroy()
{
    delete nam;
}

void ApiClient::requestMenu()
{
    QStringList names;
    QStringList sizes;
    QList<int> ids;
    QList<double> prices;
    QNetworkRequest request(QUrl(urlbase + "menu/all"));
    qDebug() << request.url();
    QNetworkReply *reply = nam->get(request);
    while(!reply->isFinished()) {
        qApp->processEvents();
    }
    QByteArray response_data = reply->readAll();
    QJsonDocument json = QJsonDocument::fromJson(response_data);
    QJsonArray menu = json.array();

    for(QJsonValue val : menu) {
        QJsonObject beverage = val.toObject();
        names.append(beverage.value("name").toString());
        ids.append(beverage.value("_Id").toString().toInt());
        prices.append(beverage.value("price").toDouble());
        sizes.append(""); //TODO read volume from api
    }

    emit menuResponse(ids, names, sizes, prices);
    delete reply;
}

void ApiClient::requestTables()
{
    QStringList names;
    QList<int> ids;
    QList<bool> waiting;
    QNetworkRequest request(QUrl(urlbase + "table/all"));
    qDebug() << request.url();
    QNetworkReply *reply = nam->get(request);
    while (!reply->isFinished())
    {
        qApp->processEvents();
    }
    QByteArray response_data = reply->readAll();
    QJsonDocument json = QJsonDocument::fromJson(response_data);

    QJsonArray tables = json.array();
    for(QJsonValue table : tables) {
        QJsonObject to = table.toObject();
        ids.append(to.value("_Id").toString().toInt());
        names.append(to.value("name").toString());
        waiting.append(!to.value("orders").toArray().isEmpty());
    }

    emit tablesResponse(ids, names, waiting);

    delete reply;
}

void ApiClient::requestOrders(int tableId)
{
    QList<QPair<int,QList<int>>> ordersToReturn;


    QNetworkRequest request(QUrl(urlbase + "table/orders/tableId=" + QString::number(tableId)));
    qDebug() << request.url();
    QNetworkReply *reply = nam->get(request);
    while (!reply->isFinished())
    {
        qApp->processEvents();
    }
    QByteArray response_data = reply->readAll();
    QJsonDocument json = QJsonDocument::fromJson(response_data);
    QJsonArray orders = json.array();
    for(QJsonValue ov : orders) {
        QPair<int,QList<int>> ord;
        QJsonObject oo = ov.toObject();
        QJsonArray bevs = oo.value("menuItemId").toArray();
        for(QJsonValue v : bevs) {
            ord.second.append(v.toString().toInt());
        }
        ord.first = oo.value("_Id").toString().toInt();
        ordersToReturn.append(ord);
    }
    emit ordersResponse(ordersToReturn);
    delete reply;
}

void ApiClient::fulfillOrders(QList<QPair<int, QPair<bool,QList<int> > > > orders)
{
    for(const QPair<int,QPair<bool,QList<int> >> &ordBevs : orders) {
        const int &orderId = ordBevs.first;
        const QList<int> &bevs = ordBevs.second.second;
        const bool &deleteOrder = ordBevs.second.first;

        if(deleteOrder) {
            QNetworkRequest request(QUrl(urlbase + "order/remove/" + QString::number(orderId)));
            qDebug() << request.url();
            QNetworkReply *reply = nam->get(request);
            while (!reply->isFinished())
            {
                qApp->processEvents();
            }
            delete reply;
        }
        else {
            for(int bevId : bevs) {
                QNetworkRequest request(QUrl(urlbase + "order/removeMenuItem/orderId=" + QString::number(orderId) + "&menuId=" + QString::number(bevId)));
                QNetworkReply *reply = nam->get(request);
                qDebug() << request.url();
                while (!reply->isFinished())
                {
                    qApp->processEvents();
                }
                delete reply;
            }
        }
    }
    emit fulfillOrdersFinished();
}

void ApiClient::createOrder(int tableId, QList<int> menuIds)
{
    if(menuIds.isEmpty()) {
        return;
    }
    QString url(urlbase+"order/new/tableId="+QString::number(tableId)+"&menu=");

    for(int mid : menuIds) {
        url += QString::number(mid);
        url += ",";
    }
    url.chop(1);

    QNetworkRequest request((QUrl(url)));
    qDebug() << request.url();
    QNetworkReply *reply = nam->get(request);
    while (!reply->isFinished())
    {
        qApp->processEvents();
    }
    delete reply;
    emit createOrderFinished();
}
