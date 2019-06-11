#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "backend.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    BackEnd backend;

//    qmlRegisterType<BackEnd>("cfb.backend", 1, 0, "BackEnd");
//    qmlRegisterUncreatableType<BackEnd>("Shared", 1, 0, "BackEnd", "error");

    qRegisterMetaType<QList<QPair<int,QList<int> > >>("QList<QPair<int,QList<int> > >");
    qRegisterMetaType<QList<QPair<int, QPair<bool, QList<int> > > >>("QList<QPair<int, QPair<bool, QList<int> > > >");

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("backend", &backend);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
