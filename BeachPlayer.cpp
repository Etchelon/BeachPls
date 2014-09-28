#include "BeachPlayer.hpp"
#include <QDebug>
#include <QDateTime>

BeachPlayer::BeachPlayer(QObject* parent)
	: QObject{ parent }
{
}

BeachPlayer::BeachPlayer(QString name, double height)
	: m_name{ name }, m_height{ height },
	  m_attack{ 0 },
	  m_pass{ 0 },
	  m_serve{ 0 },
	  m_set{ 0 },
	  m_reaction{ 0 },
	  m_discipline{ 0 }
{
}

bool BeachPlayer::compiled() const
{
	return  m_attack != 0 &&
			m_pass != 0 &&
			m_serve != 0 &&
			m_set != 0 &&
			m_reaction != 0 &&
			m_discipline != 0;
}

void BeachPlayer::load_from_json(const QJsonObject& json)
{
	m_name = json.value("name").toString();
	m_height = json.value("height").toDouble();

	QJsonObject ratings = json.value("ratings").toObject();
	m_attack = ratings.value("attack").toInt();
	m_pass = ratings.value("pass").toInt();
	m_serve = ratings.value("serve").toInt();
	m_set = ratings.value("set").toInt();
	m_reaction = ratings.value("reaction").toInt();
	m_discipline = ratings.value("discipline").toInt();
}

QJsonObject BeachPlayer::get_json() const
{
	QJsonObject ret;
	ret["name"] = m_name;
	ret["height"] = m_height;

	QJsonObject ratings;
	ratings["attack"] = m_attack;
	ratings["pass"] = m_pass;
	ratings["serve"] = m_serve;
	ratings["set"] = m_set;
	ratings["reaction"] = m_reaction;
	ratings["discipline"] = m_discipline;

	ret["ratings"] = ratings;

	return ret;
}

