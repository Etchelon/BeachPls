#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class BeachPlayer;

class Engine : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QList<QObject*> players READ players CONSTANT)

public:
	enum class Action
	{
		GetHelloWorld,
		Login,
		Logout,
		CheckLoggedIn
	};

private:
	static const QString AppFolder;
	static const QString DbFile;
	QList<QObject*> m_players;

	QNetworkAccessManager m_manager;
	QString m_beachPlsWebServiceUrl = "http://localhost:57370/WebServices/Service.asmx/";

public:
	explicit Engine(QObject* parent = nullptr);
	~Engine();

	QList<QObject*> players() const;

private:
	void load_from_db();
	void save_to_db();
	QByteArray get_json_data() const;

signals:

public Q_SLOTS:
	void login();
	void submit_to_server();

private slots:
	void reply_ready(QNetworkReply* reply);
};

#endif // ENGINE_HPP
