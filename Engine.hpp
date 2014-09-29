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
	Q_PROPERTY(QString username READ username WRITE set_username NOTIFY usernameChanged)
	Q_PROPERTY(QString password READ password WRITE set_password NOTIFY passwordChanged)

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
	static const QString SettingsFile;
	QList<QObject*> m_players;
	QString m_username = QString::null;
	QString m_password = QString::null;

	QNetworkAccessManager m_manager;
//	QString m_beachPlsWebServiceUrl = "http://localhost:57370/WebServices/Service.asmx/";
	QString m_beachPlsWebServiceUrl = "http://beachpls.azurewebsites.net/WebServices/Service.asmx/";

public:
	explicit Engine(QObject* parent = nullptr);
	~Engine();

	QList<QObject*> players() const;
	QString username() const;
	QString password() const;
	void set_username(QString newUsername);
	void set_password(QString newPassword);

private:
	void load_from_db();
	void save_to_db();
	QByteArray get_json_data() const;

signals:
	void usernameChanged();
	void passwordChanged();
	void loginSuccess();
	void loginFailed();
	void ratingsSubmittedSuccessfully();
	void ratingsNotSubmitted();

public Q_SLOTS:
	void login();
	void submit_to_server();

private slots:
	void reply_ready(QNetworkReply* reply);
};

#endif // ENGINE_HPP
