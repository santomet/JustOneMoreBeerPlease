#ifndef ORDERINFO_H
#define ORDERINFO_H

#include <QObject>
#include <QVariant>
#include "beverageinfo.h"


class OrderInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant beverages READ beverages NOTIFY beveragesChanged)
    Q_PROPERTY(QString table READ table WRITE setTable NOTIFY tableNameChanged)
    Q_PROPERTY(QString user READ user WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)


public:

    explicit OrderInfo(QObject *parent = nullptr);
//    ~OrderInfo();
    void addBeverage(const BeverageInfo* bi);
    void addBeverage(int id, const QString &name, const QString &size, float price, int ordercount);


    bool isEmpty() {return mContents.isEmpty();}
    QList<QObject*> getBeverageList() {return mContents;}


signals:
    void beveragesChanged();
    void tableNameChanged();
    void userNameChanged();
    void idChanged();

public slots:
    QVariant beverages();
    QString table();
    QString user();
    int id();

    QList<QObject*> beveragesList();

    void setTable(const QString &tablename);
    void setUserName(const QString &username);
    void setId(int id);

private:
    QString mTable;
    QList<QObject*> mContents;
    QString mUser;
    int mId{0};
};

#endif // ORDERINFO_H
