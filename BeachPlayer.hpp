#ifndef BEACHPLAYER_HPP
#define BEACHPLAYER_HPP

#include <QObject>
#include "Macros.hpp"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

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

	bool compiled() const;

signals:

public:
	void load_from_xml(QXmlStreamReader& reader);
	void save_to_xml(QXmlStreamWriter& writer) const;
};

#endif // BEACHPLAYER_HPP
