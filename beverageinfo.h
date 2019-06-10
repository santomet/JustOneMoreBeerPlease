#ifndef BEVERAGEINFO_H
#define BEVERAGEINFO_H

#include <QObject>

class BeverageInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(float price READ price WRITE setPrice NOTIFY priceChanged)

    Q_PROPERTY(int orderCount READ orderCount WRITE setOrderCount NOTIFY orderCountChanged)
    Q_PROPERTY(int fulfilled READ fulfilled WRITE setFulfilled NOTIFY fulfilledChanged)

public:
    explicit BeverageInfo(QObject *parent = nullptr);

signals:
    void nameChanged();
    void sizeChanged();
    void priceChanged();
    void idChanged();
    void orderCountChanged();
    void fulfilledChanged();

public slots:
    int id() const {return mId;}
    QString name() const {return mName;}
    QString size() const {return mSize;}
    float price() const {return mPrice;}


    int orderCount() const {return mOrderCount;}
    int fulfilled() const {return mFulfilledOrders;}

    void setId(const int &id) {mId = id; emit idChanged();}
    void setName(const QString &name) {mName = name; emit nameChanged();}
    void setSize(const QString &size) {mSize = size; emit sizeChanged();}
    void setPrice(const float &price) {mPrice = price; emit priceChanged();}

    void setOrderCount(const int &oc) {mOrderCount = oc; emit orderCountChanged();}
    void setFulfilled(const int &fc) {mFulfilledOrders = fc; emit fulfilledChanged();}

private:
    int mId{0};
    QString mName{"Pilsner Urquell"};
    QString mSize{"0.5l"};
    float mPrice{49};

    int mOrderCount{0};
    int mFulfilledOrders{0};
};

#endif // BEVERAGEINFO_H
