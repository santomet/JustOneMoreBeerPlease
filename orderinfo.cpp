#include "orderinfo.h"

OrderInfo::OrderInfo(QObject *parent) : QObject(parent)
{

}

//OrderInfo::~OrderInfo()
//{
//}

void OrderInfo::addBeverage(const BeverageInfo *bi)
{
    addBeverage(bi->id(), bi->name(), bi->size(), bi->price(), bi->orderCount());
}

void OrderInfo::addBeverage(int id, const QString &name, const QString &size, float price, int ordercount)
{
    if(ordercount < 1) {
        return;
    }

    for(QObject *bo : mContents) {
        BeverageInfo *bevi = qobject_cast<BeverageInfo*>(bo);
        if(bevi->id() == id) {
            bevi->setOrderCount(bevi->orderCount()+ordercount);
            emit beveragesChanged();
            return;
        }
    }

    BeverageInfo *bi = new BeverageInfo(this);
    bi->setId(id); bi->setName(name); bi->setSize(size); bi->setPrice(price); bi->setOrderCount(ordercount);
    mContents.append(bi);
    emit beveragesChanged();
}

QVariant OrderInfo::beverages()
{
    return QVariant::fromValue(mContents);
}

QString OrderInfo::table()
{
    return mTable;
}

QString OrderInfo::user()
{
    return mUser;
}

int OrderInfo::id()
{
    return mId;
}

QList<QObject *> OrderInfo::beveragesList()
{
    return mContents;
}

void OrderInfo::setTable(const QString &tablename)
{
    mTable = tablename;
    emit tableNameChanged();
}

void OrderInfo::setUserName(const QString &username)
{
    mUser = username;
    emit userNameChanged();
}

void OrderInfo::setId(int id)
{
    mId = id;
    emit idChanged();
}
