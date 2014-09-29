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
const QString Engine::SettingsFile = Engine::AppFolder + "beachpls.config";

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

	if (!QFile::exists(SettingsFile))
	{
		QFile settingsFile{ SettingsFile };
		settingsFile.open(QIODevice::WriteOnly);
	}
	else
	{
		QFile settingsFile{ SettingsFile };
		settingsFile.open(QIODevice::ReadOnly);

		QString line{ settingsFile.readAll() };
		auto splits = line.split(';');
		if (splits.size() == 2)
		{
			m_username = splits[0];
			m_password = splits[1];
		}
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

	if (m_username != QString::null)
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

QString Engine::username() const
{
	return m_username;
}

QString Engine::password() const
{
	return m_password;
}

void Engine::set_username(QString newUsername)
{
	if (m_username != newUsername)
	{
		m_username = newUsername;
		emit usernameChanged();
	}
}

void Engine::set_password(QString newPassword)
{
	if (m_password != newPassword)
	{
		m_password = newPassword;
		emit passwordChanged();
	}
}

void Engine::load_from_db()
{
	QFile databaseFile{ DbFile };
	if (!databaseFile.open(QIODevice::ReadOnly))
		throw std::runtime_error("Could not open the database file. Try with a restart or check if the db.xml file is in the folder");

	QString file{ databaseFile.readAll() };
	qDebug() << "Save file content:\n" << file << endl;

	static const QRegularExpression regex1{ R"((?<record>\s*\[Timestamp='[a-zA-Z0-9: ]+'\]\s*([\[\]{}\",\.\w\d: \n]+)\s*\[EndTimestamp\]\s*))" };
	static const QRegularExpression regex2{ R"(\[Timestamp='(?<timestamp>[a-zA-Z0-9: ]+)'\](?<content>[\[\]{}\",\.\w\d: \n]+)\[EndTimestamp\])" };

	QRegularExpressionMatchIterator it = regex1.globalMatch(file);
	while (it.hasNext())
	{
		QString record = it.next().captured("record");
		qDebug() << "Batch of data:\n" << record << endl;
		auto match = regex2.match(record);

		QDateTime date = QDateTime::fromString(match.captured("timestamp"));
		QJsonArray data = QJsonDocument::fromJson(match.captured("content").toUtf8()).array();
		for (QJsonValue val : data)
		{
			QJsonObject obj = val.toObject();
			qDebug() << "Player in this batch of data: " << obj.value("name").toString() << endl;

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
	if (m_username != QString::null)
	{
		QFile settingsFile{ SettingsFile };
		if (!settingsFile.open(QIODevice::WriteOnly))
			throw std::runtime_error{ "Could not open the temp file for saving the image to the database" };

		settingsFile.write((m_username + ';' + m_password).toUtf8());
		settingsFile.close();
	}

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
	params["username"] = m_username;
	params["password"] = m_password;

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

	QString requestUrl = reply->request().url().toString();
	int lastSlash = requestUrl.lastIndexOf('/');
	QString action = requestUrl.right(requestUrl.length() - (lastSlash + 1));
	qDebug() << "Action: " << action << endl;

	if (action == "Login")
	{
		bool success = doc.object().value("d").toBool();
		if (success)
			emit loginSuccess();
		else
			emit loginFailed();
	}
	else if (action == "SubmitRatings")
	{
		bool success = doc.object().value("d").toBool();
		if (success)
			emit ratingsSubmittedSuccessfully();
		else
			emit ratingsNotSubmitted();
	}

	return;
}
