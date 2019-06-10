#ifndef TABLEINFO_H
#define TABLEINFO_H

#include <QObject>

class TableInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString tableName READ name NOTIFY nameChanged)
    Q_PROPERTY(bool waitingOrder READ isWaitingOrder WRITE setWaitingOrder NOTIFY isWaitingOrderChanged)

public:
    explicit TableInfo(QObject *parent = nullptr);

    QString name();
    bool isWaitingOrder();

signals:
    void nameChanged();
    void isWaitingOrderChanged();

public slots:
    void setName(const QString &name);
    void setWaitingOrder(bool waiting = true);

private:
    QString mName{"1"};
    bool waitingOrder{false};


};

#endif // TABLEINFO_H
