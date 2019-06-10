#include "tableinfo.h"

TableInfo::TableInfo(QObject *parent) : QObject(parent)
{

}

QString TableInfo::name()
{
    return mName;
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
