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
