#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <QObject>

class BeachPlayer;

class Engine : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QList<QObject*> players READ players CONSTANT)

private:
	static const QString AppFolder;
	static const QString DbFile;
	QList<QObject*> m_players;

public:
	explicit Engine(QObject* parent = nullptr);
	~Engine();

	QList<QObject*> players() const;

private:
	void load_from_db();
	void save_to_db();

signals:

public slots:

};

#endif // ENGINE_HPP
