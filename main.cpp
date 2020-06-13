#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "gamearea.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    GameArea gameArea;
    qmlRegisterType<GameArea>("Game", 1, 0, "GameAreamModel");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
