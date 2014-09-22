#include "Engine.hpp"
#include "BeachPlayer.hpp"
#include <QDir>
#include <QFile>
#include <QDateTime>

const QString Engine::AppFolder = QDir::homePath() + "/AppData/Roaming/BeachPls/";
const QString Engine::DbFile = Engine::AppFolder + "db.xml";

Engine::Engine(QObject* parent)
	: QObject{ parent }
{
	QDir path{ AppFolder };

	// Check for the existence of support folders; create if they don't exist
	if (!path.exists())
		path.mkpath(".");

	if (!QFile::exists(DbFile))
	{
		QFile databaseFile{ DbFile };
		databaseFile.open(QIODevice::WriteOnly);
	}

	m_players.append(new BeachPlayer{ "Fish", 1.74 });
	m_players.append(new BeachPlayer{ "Vale", 1.65 });
	m_players.append(new BeachPlayer{ "Phil", 1.70 });
	m_players.append(new BeachPlayer{ "Regia", 1.80 });
	m_players.append(new BeachPlayer{ "Luca", 1.96 });
	m_players.append(new BeachPlayer{ "Bad", 1.72 });
	m_players.append(new BeachPlayer{ "Peppo", 1.72 });
	m_players.append(new BeachPlayer{ "Roby", 1.74 });
	m_players.append(new BeachPlayer{ "Zilli", 1.75 });
	m_players.append(new BeachPlayer{ "Kuji", 1.78 });
	m_players.append(new BeachPlayer{ "Ila", 1.65 });
	m_players.append(new BeachPlayer{ "Mari", 1.78 });
	m_players.append(new BeachPlayer{ "Giulia", 1.74 });
	m_players.append(new BeachPlayer{ "Alby", 1.82 });
	m_players.append(new BeachPlayer{ "Silvia", 1.68 });
	m_players.append(new BeachPlayer{ "Mema", 1.70 });
	m_players.append(new BeachPlayer{ "Moni", 1.65 });
	m_players.append(new BeachPlayer{ "Dado", 1.76 });
	m_players.append(new BeachPlayer{ "Fabbro", 1.70 });
}

Engine::~Engine()
{
	save_to_db();
}

QList<QObject*> Engine::players() const
{
	return m_players;
}

void Engine::load_from_db()
{
	QFile databaseFile{ DbFile };
	if (!databaseFile.open(QIODevice::ReadOnly))
		throw std::runtime_error("Could not open the database file for saving the image. Try with a restart or check if the database.xml file is in the folder");

	QXmlStreamReader reader{ &databaseFile };

	while (!reader.atEnd())
	{
		if (reader.isStartElement() && reader.name() == "Player")
		{
			BeachPlayer* player = new BeachPlayer;
			player->load_from_xml(reader);

			m_players.append(player);
		}

		reader.readNext();
	}
}

void Engine::save_to_db()
{
	bool save = false;
	for (QObject* player : m_players)
		if ((static_cast<BeachPlayer*>(player))->compiled())
		{
			save = true;
			break;
		}

	if (!save)
		return;

	// Open the database file to append the new image info
	QFile databaseFile{ DbFile };

	QFile tempFile{ AppFolder + "temp.xml" };
	if (!tempFile.open(QIODevice::WriteOnly))
		throw std::runtime_error{ "Could not open the temp file for saving the image to the database" };

	QXmlStreamWriter writer{ &tempFile };
	writer.setAutoFormatting(true);
	writer.setAutoFormattingIndent(4);

	// Start writing
	writer.writeStartDocument();
	writer.writeStartElement("Players");
	writer.writeAttribute("Timestamp", QDateTime::currentDateTime().toString());

	// Appended here the image data
	for (QObject* player : m_players)
		(static_cast<BeachPlayer*>(player))->save_to_xml(writer);

	writer.writeEndElement();				// Players
	writer.writeEndDocument();				// Document

	// Append the old data
	if (databaseFile.exists() && databaseFile.open(QIODevice::ReadOnly))
	{
		databaseFile.readLine();
		tempFile.write("\n");
		tempFile.write(databaseFile.readAll());
	}

	// Switch the temporary with the old file
	tempFile.close();

	if (databaseFile.exists() && !databaseFile.rename(AppFolder + "olddb.xml"))
	{
		tempFile.remove();
		throw std::runtime_error{ "Could not rename the old database file for deletion; couldn't save the info about images to the database" };
	}

	if (!tempFile.rename(DbFile))
	{
		databaseFile.rename(DbFile);
		throw std::runtime_error{ "Could not rename the temp file for deletion; couldn't save the info about images to the database" };
	}
	else
		databaseFile.remove();
}
