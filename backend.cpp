#include "backend.h"

BackEnd::BackEnd(QObject *parent) :
    QObject(parent)
{

    if(demo) {
    QStringList l{"Table 1","Table 2","Table 3", "Table 4", "Table 5", "Table 6", "Outside 1", "Outside 2", "Outside 3", "Outside 4"};
    QList<int> ids;
    QList<bool> waiting;
    for(int i = 1;i<=l.count();i++) {
        ids.append(i);
        waiting.append(false);
    }
    setTables(ids, l, waiting);
//    l.removeOne("Table 2");
//    setOrderWaitingTables(l, true);

    addBeverage(0, "Pilsner Urquell", "0.5l", 49);
    addBeverage(1, "Gambrinus 10", "0.5l", 33);
    addBeverage(2, "Starobrno Drak", "0.5l", 33);
    addBeverage(3, "Budvar 12", "0,5l", 35);
    addBeverage(4, "Pilsner Urquell", "0.3l", 33);
    addBeverage(5, "Gambrinus 10", "0.3l", 25);
    addBeverage(6, "Starobrno Drak", "0.3l", 23);
    addBeverage(7, "Budvar 12", "0,3l", 25);
    }
    else {
     client = new ApiClient();
     connect(this, &BackEnd::requestTablesSignal, client, &ApiClient::requestTables);
     connect(client, &ApiClient::tablesResponse, this, &BackEnd::setTables);

     connect(this, &BackEnd::requestMenuSignal, client, &ApiClient::requestMenu);
     connect(client, &ApiClient::menuResponse, this, &BackEnd::setBeverages);

     connect(this, &BackEnd::requestOrdersSignal, client, &ApiClient::requestOrders);
     connect(client, &ApiClient::ordersResponse, this, &BackEnd::setOrders);

     connect(this, &BackEnd::requestFulfillOrdersSignal, client, &ApiClient::fulfillOrders);
     connect(client, &ApiClient::fulfillOrdersFinished, this, &BackEnd::requestOrdersForActiveTable);

     connect(this, &BackEnd::requestCreateOrder, client, &ApiClient::createOrder);
     connect(client, &ApiClient::createOrderFinished, this, &BackEnd::requestOrdersForActiveTable);

     emit requestTablesSignal();
     emit requestMenuSignal();
    }
}

BackEnd::~BackEnd()
{
    delete client;
}

QString BackEnd::userName()
{
    return m_userName;
}

QVariant BackEnd::tables()
{
    return QVariant::fromValue(mTables);
}

QVariant BackEnd::beverages()
{
    return QVariant::fromValue(mBeverages);
}

QVariant BackEnd::ordersForActiveTable()
{
    QList<QObject*> fl;
    for(OrderInfo *oi : mOrders){
        if(oi->table() == mActiveTable) {
            fl.append(oi->getBeverageList());
            qDebug() << "added beverage list for " << mActiveTable << fl;
        }
    }
    costForActiveTable = 0;
    for(QObject *bo : fl) {
        BeverageInfo *bi = qobject_cast<BeverageInfo*>(bo);
        costForActiveTable += bi->price() * bi->orderCount();
    }
    emit costForActiveTableChanged();
    return QVariant::fromValue(fl);
}

QString BackEnd::activeTable()
{
    return mActiveTable;
}

int BackEnd::activeTableId()
{
    return mActiveTableId;
}

float BackEnd::activeTableCost()
{
    return costForActiveTable;
}

void BackEnd::setUserName(const QString &userName)
{
    if (userName == m_userName)
        return;
    qDebug() << "HAHAHAHHAAHHA";
    m_userName = userName;
    emit userNameChanged();
}

void BackEnd::setActiveTable(const QString &tableName)
{
    mActiveTable = tableName;
    qDebug() << "setting active table to" << tableName;
    emit activeTableChanged();
}

void BackEnd::setActiveTableId(const int id)
{
    mActiveTableId = id;
    emit activeTableIdChanged();
}

void BackEnd::setTables(QList<int> ids, QStringList names, QList<bool> waitingOrders)
{
    qDeleteAll(mTables);
    mTables.clear();
    if(names.count() != ids.count() || names.count() != waitingOrders.count()){
        return;
    }
    for(int i = 0; i<names.count();++i) {
        TableInfo *ti = new TableInfo(this);
        ti->setName(names.at(i));
        ti->setId(ids.at(i)); qDebug() << "Adding table id " << ids.at(i);
        ti->setWaitingOrder(waitingOrders.at(i));
        mTables.append(ti);
    }
    emit tablesChanged();
}

void BackEnd::clearBeverages()
{
    qDeleteAll(mBeverages);
    mBeverages.clear();
    emit beveragesChanged();
}

void BackEnd::setBeverages(QList<int> id, QStringList names, QStringList sizes, QList<double> prices)
{
    qDeleteAll(mBeverages);
    mBeverages.clear();

    if(id.count() != names.count() || id.count() != sizes.count() || id.count()!= prices.count()) {
        emit beveragesChanged();
        return;
    }

    for(int i = 0; i<id.count(); ++i) {
        addBeverage(id.at(i),names.at(i),sizes.at(i),prices.at(i));
    }

    emit beveragesChanged();
}

void BackEnd::addBeverage(int id, QString name, QString size, float price)
{
    BeverageInfo *bi = new BeverageInfo(this);
    bi->setId(id);
    bi->setName(name);
    bi->setSize(size);
    bi->setPrice(price);
    mBeverages.append(bi);
}

void BackEnd::setOrders(QList<QPair<int, QList<int> > > orders)
{
    qDeleteAll(mOrders);
    mOrders.clear();

    for(const QPair<int,QList<int>> &p : orders) {
        OrderInfo *oi = new OrderInfo(this);
        oi->setId(p.first);
        oi->setTable(mActiveTable);
        for(int id : p.second) {
            for(QObject *bo : mBeverages) {
                BeverageInfo *bi = qobject_cast<BeverageInfo*>(bo);
                if(bi->id() == id) {
                    oi->addBeverage(bi->id(), bi->name(), bi->size(), bi->price(), 1); //do this shit a many times as needed
                }
            }
        }
        mOrders.append(oi);
    }
    emit ordersChanged();
}

void BackEnd::makeOrder()
{
    if(demo) {
        OrderInfo *oi = new OrderInfo(this);
        oi->setTable(mActiveTable);
        for(QObject *o : mBeverages) {
            if(!o) {
                continue;
            }
            BeverageInfo *bi = qobject_cast<BeverageInfo*>(o);
            if(bi->orderCount() > 0) {
                oi->addBeverage(bi);
            }
            bi->setOrderCount(0);
        }
        if(oi->isEmpty()) {
            delete oi;
        }
        else {

            for(QObject *to : mTables) {
                if(to->property("tableName").toString() == mActiveTable) {
                    to->setProperty("waitingOrder", true);
                }
            }
            mOrders.append(oi);
            emit ordersChanged();


        }
    }
    else {
        QList<int> bevIds;
        for(QObject *o : mBeverages) {
            if(!o) {
                continue;
            }
            BeverageInfo *bi = qobject_cast<BeverageInfo*>(o);
            for(int i = 0;i<bi->orderCount();++i) {
                bevIds.append(bi->id());
            }
            bi->setOrderCount(0);
        }

        emit requestCreateOrder(mActiveTableId, bevIds);
    }
}

void BackEnd::fulfill()
{
    if(demo) {
    QList<OrderInfo*> toDelete;
    QStringList tablesStillUnfulfilled;
    for(OrderInfo *i : mOrders) {
        bool removeIt = true;
        for(QObject *bo : i->beveragesList()) {
            BeverageInfo *bi = qobject_cast<BeverageInfo*>(bo);
            if(bi->fulfilled() > 0) {
                bi->setOrderCount(bi->orderCount()-bi->fulfilled());
                bi->setFulfilled(0);
            }
            if(bi->orderCount() > 0) {
                if(!tablesStillUnfulfilled.contains(i->table())) {
                    tablesStillUnfulfilled.append(i->table());
                }
                removeIt = false;
            }
        }
        if(removeIt) {
            toDelete.append(i);
        }
    }
    for(OrderInfo *i : toDelete) {
        mOrders.removeAll(i);
    }
    qDeleteAll(toDelete);
    emit ordersChanged();
    setOrderWaitingTables(tablesStillUnfulfilled, true, true);
    }
    else {
        QList<QPair<int, QPair<bool,QList<int> > > > orders;
        for(OrderInfo *i : mOrders) {
            QPair<int, QPair<bool,QList<int> > >  order;
            order.first = i->id();
            for(QObject *bo : i->beveragesList()) {
                BeverageInfo *bi = qobject_cast<BeverageInfo*>(bo);
                for(int i = 0; i<bi->fulfilled();++i) {
                    order.second.second.append(bi->id());
                }
                if(bi->fulfilled() == bi->orderCount()) {
                    order.second.first = true;
                }
                bi->setFulfilled(0);
            }
            orders.append(order);
        }
        emit requestFulfillOrdersSignal(orders);
    }
}

void BackEnd::requestTables()
{
    emit requestTablesSignal();
}

void BackEnd::requestOrdersForActiveTable()
{
    emit requestOrdersSignal(mActiveTableId);
}

void BackEnd::requestMenu()
{
    emit requestMenuSignal();
}

void BackEnd::setOrderWaitingTables(QStringList tableNames, bool waiting, bool revertOthers)
{
    for(QObject* table : mTables) {
        if(!table) {
            continue;
        }
        TableInfo *i = qobject_cast<TableInfo*>(table);
        if(tableNames.contains(i->name())) {
            i->setWaitingOrder(waiting);
        }
        else if(revertOthers) {
            i->setWaitingOrder(!waiting);
        }
    }
}
