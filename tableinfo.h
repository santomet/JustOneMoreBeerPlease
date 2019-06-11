#ifndef TABLEINFO_H
#define TABLEINFO_H

#include <QObject>

class TableInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString tableName READ name NOTIFY nameChanged)
    Q_PROPERTY(bool waitingOrder READ isWaitingOrder WRITE setWaitingOrder NOTIFY isWaitingOrderChanged)
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)

public:
    explicit TableInfo(QObject *parent = nullptr);

    QString name();
    int id();
    bool isWaitingOrder();

signals:
    void nameChanged();
    void isWaitingOrderChanged();
    void idChanged();

public slots:
    void setName(const QString &name);
    void setWaitingOrder(bool waiting = true);
    void setId(int id);

private:
    QString mName{"1"};
    int mId{0};
    bool waitingOrder{false};


};

#endif // TABLEINFO_H
