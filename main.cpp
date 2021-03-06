#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml/qqml.h>
#include <QIcon>
#include "BeachPlayer.hpp"
#include "Engine.hpp"

int main(int argc, char *argv[])
{
	qmlRegisterType<BeachPlayer>("Logic", 1, 0, "Player");
	qmlRegisterType<Engine>("Logic", 1, 0, "Engine");

	QApplication app(argc, argv);
	app.setApplicationVersion("0.5");
	app.setWindowIcon(QIcon{ "AppIcon.ico" });

	QQmlApplicationEngine engine;
	engine.load(QUrl(QStringLiteral("qrc:///BeachPlsUI.qml")));

	return app.exec();
}
