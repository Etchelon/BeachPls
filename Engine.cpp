#include "Engine.hpp"
#include "BeachPlayer.hpp"
#include <QDir>
#include <QFile>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonArray>
#include <QRegExp>
#include <QRegularExpression>
#include <QDebug>

const QString Engine::AppFolder = QDir::homePath() + "/AppData/Roaming/BeachPls/";
const QString Engine::DbFile = Engine::AppFolder + "db.xml";

Engine::Engine(QObject* parent)
	: QObject{ parent }
{
	connect(&m_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(reply_ready(QNetworkReply*)));

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

	load_from_db();

	login();
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
		throw std::runtime_error("Could not open the database file. Try with a restart or check if the db.xml file is in the folder");

	QString file{ databaseFile.readAll() };

	static const QRegularExpression regex1{ R"((?<record>\s*\[Timestamp='([a-zA-Z0-9: ]+)'\]([\[\]{}\",\w\d: \n]+)\[EndTimestamp\]\s*))" };
	static const QRegularExpression regex2{ R"(\[Timestamp='(?<timestamp>[a-zA-Z0-9: ]+)'\](?<content>[\[\]{}\",\w\d: \n]+)\[EndTimestamp\])" };

	QRegularExpressionMatchIterator it = regex1.globalMatch(file);
	while (it.hasNext())
	{
		QString record = it.next().captured("record");
		auto match = regex2.match(record);

		QDateTime date = QDateTime::fromString(match.captured("timestamp"));
		QJsonArray data = QJsonDocument::fromJson(match.captured("content").toUtf8()).array();
		for (QJsonValue val : data)
		{
			QJsonObject obj = val.toObject();

			auto playerPtr = std::find_if(std::begin(m_players), std::end(m_players), [&obj](QObject* p){
				return (static_cast<BeachPlayer*>(p))->name() == obj.value("name").toString();
			});

			BeachPlayer* player = static_cast<BeachPlayer*>(*playerPtr);
			if (!player->compiled())
				player->load_from_json(obj);
		}
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

	tempFile.write(QString{ "[Timestamp='%1']\n" }.arg(QDateTime::currentDateTime().toString()).toStdString().c_str());
	tempFile.write(get_json_data());
	tempFile.write(QString{ "[EndTimestamp]" }.arg(QDateTime::currentDateTime().toString()).toStdString().c_str());

	// Append the old data
	if (databaseFile.exists() && databaseFile.open(QIODevice::ReadOnly))
	{
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

QByteArray Engine::get_json_data() const
{
	QJsonArray data;
	for (QObject* m_player : m_players)
	{
		BeachPlayer* player = static_cast<BeachPlayer*>(m_player);
		if (player->compiled())
			data.append(player->get_json());
	}

	return QJsonDocument(data).toJson();
}

void Engine::login()
{
	QJsonObject params;
	params["username"] = "Etchelon";
	params["password"] = "caccapupu85";

	QNetworkRequest request{ m_beachPlsWebServiceUrl + "Login" };
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	m_manager.post(request, QJsonDocument(params).toJson());
}

void Engine::submit_to_server()
{
	QJsonObject params;
	params["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
	params["data"] = QString{ get_json_data() };

	QNetworkRequest request{ m_beachPlsWebServiceUrl + "SubmitRatings" };
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	m_manager.post(request, QJsonDocument(params).toJson());
}

void Engine::reply_ready(QNetworkReply* reply)
{
	auto rawReply = reply->readAll();
	qDebug() << "RawReply: " << rawReply << endl << endl;

	QJsonParseError* error = nullptr;
	auto doc = QJsonDocument::fromJson(rawReply, error);

	if (error)
		qDebug() << "PARSE ERROR! " << error->errorString() << endl << endl;

//	if (doc.isObject())
//	{
//		QJsonObject obj = doc.object();

//		if (!obj.empty() && obj.value("success").toBool())
//		{
//			QJsonValue date = obj.value("date");

//			if (date.isString())
//				qDebug() << "Retrieved date: " << QDateTime::fromString(date.toString(), Qt::DateFormat::ISODate) << endl << endl;
//		}
//	}

	return;
}
