#include "BeachPlayer.hpp"

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

void BeachPlayer::load_from_xml(QXmlStreamReader& reader)
{
	reader.readNextStartElement();

	// Assume this function is called after encountering the opening <Player> tag
	while (!reader.atEnd())
	{
		if (reader.isStartElement() && reader.name() == "Name")
			m_name = reader.readElementText();

		else if (reader.isStartElement() && reader.name() == "Height")
			m_height = reader.readElementText().toDouble();

		else if (reader.isStartElement() && reader.name() == "Attack")
			m_attack = reader.readElementText().toInt();

		else if (reader.isStartElement() && reader.name() == "Pass")
			m_pass = reader.readElementText().toInt();

		else if (reader.isStartElement() && reader.name() == "Serve")
			m_serve = reader.readElementText().toInt();

		else if (reader.isStartElement() && reader.name() == "Set")
			m_set = reader.readElementText().toInt();

		else if (reader.isStartElement() && reader.name() == "Reaction")
			m_reaction = reader.readElementText().toInt();

		else if (reader.isStartElement() && reader.name() == "Discipline")
			m_discipline = reader.readElementText().toInt();

		else
			throw std::runtime_error{ "Unknown tag while reading image from xml; tag: " + reader.readElementText().toStdString() };

		reader.readNextStartElement();
	}
}

void BeachPlayer::save_to_xml(QXmlStreamWriter& writer) const
{
	if (!compiled())
		return;

	writer.writeStartElement("Player");

	writer.writeStartElement("Name");
	writer.writeCharacters(m_name);
	writer.writeEndElement();				// Name

	writer.writeStartElement("Height");
	writer.writeCharacters(QString::number(m_height));
	writer.writeEndElement();				// Height

	writer.writeStartElement("Attack");
	writer.writeCharacters(QString::number(m_attack));
	writer.writeEndElement();				// Attack

	writer.writeStartElement("Pass");
	writer.writeCharacters(QString::number(m_pass));
	writer.writeEndElement();				// Pass

	writer.writeStartElement("Serve");
	writer.writeCharacters(QString::number(m_serve));
	writer.writeEndElement();				// Serve

	writer.writeStartElement("Set");
	writer.writeCharacters(QString::number(m_set));
	writer.writeEndElement();				// Set

	writer.writeStartElement("Reaction");
	writer.writeCharacters(QString::number(m_reaction));
	writer.writeEndElement();				// Reaction

	writer.writeStartElement("Discipline");
	writer.writeCharacters(QString::number(m_discipline));
	writer.writeEndElement();				// Discipline

	writer.writeEndElement();				// Player
}
