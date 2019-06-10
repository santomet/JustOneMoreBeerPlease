#include "backend.h"

BackEnd::BackEnd(QObject *parent) :
    QObject(parent)
{

    if(demo) {
    QStringList l{"Table 1","Table 2","Table 3", "Table 4", "Table 5", "Table 6", "Outside 1", "Outside 2", "Outside 3", "Outside 4"};
    setTables(l);
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

void BackEnd::setTables(QStringList tableNames)
{
    qDeleteAll(mTables);
    for(const QString &name : tableNames) {
        TableInfo *ti = new TableInfo(this);
        ti->setName(name);
        mTables.append(ti);
    }
}

void BackEnd::clearBeverages()
{
    qDeleteAll(mBeverages);
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
    emit beveragesChanged();
}

void BackEnd::makeOrder()
{
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
        if(demo) {
            for(QObject *to : mTables) {
                if(to->property("tableName").toString() == mActiveTable) {
                    to->setProperty("waitingOrder", true);
                }
            }
            mOrders.append(oi);
            emit ordersChanged();
        }
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
