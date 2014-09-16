#ifndef BEACHPLAYER_HPP
#define BEACHPLAYER_HPP

#include <QObject>
#include "Macros.hpp"

class BeachPlayer : public QObject
{
	Q_OBJECT

	QPROPERTY(QString, name)
	QPROPERTY_WRITE_NOTIFY(double, height)
	QPROPERTY_WRITE_NOTIFY(int, attack)
	QPROPERTY_WRITE_NOTIFY(int, pass)
	QPROPERTY_WRITE_NOTIFY(int, set)
	QPROPERTY_WRITE_NOTIFY(int, serve)
	QPROPERTY_WRITE_NOTIFY(int, reaction)
	QPROPERTY_WRITE_NOTIFY(int, discipline);

public:
	explicit BeachPlayer(QObject* parent = nullptr);
	BeachPlayer(QString name, double height);

signals:

public slots:

};

#endif // BEACHPLAYER_HPP
