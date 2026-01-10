#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "musiccontroller.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    
    // Create and register the music controller
    MusicController musicController;
    engine.rootContext()->setContextProperty("musicController", &musicController);
    
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    engine.load(url);
    
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
