#include "tableinfo.h"

TableInfo::TableInfo(QObject *parent) : QObject(parent)
{

}

QString TableInfo::name()
{
    return mName;
}

int TableInfo::id()
{
    return mId;
}

bool TableInfo::isWaitingOrder()
{
    return waitingOrder;
}

void TableInfo::setName(const QString &name)
{
    mName = name;
    emit nameChanged();
}

void TableInfo::setWaitingOrder(bool waiting)
{
    waitingOrder = waiting;
    emit isWaitingOrderChanged();
}

void TableInfo::setId(int id)
{
    mId = id;
    emit idChanged();
}
