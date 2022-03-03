#include "rootstate.h"
#include "websocketclient.h"
#include "messagemodel.h"

#include <QFontDatabase>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<WebSocketClient>("me.blq.socketui.components", 0, 1, "WebSocketClient");
    qmlRegisterType<MessageModel>("me.blq.socketui.components", 0, 1, "MessageModel");

    QQmlApplicationEngine engine;
    QQuickStyle::setStyle("Material");
    const QUrl url(u"qrc:/socketui/qml/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    auto systemMono = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    engine.rootContext()->setContextProperty("monospaceFont", systemMono);

    auto rootState = new RootState();
    engine.rootContext()->setContextProperty("rootState", rootState);

    engine.load(url);

    return app.exec();
}
