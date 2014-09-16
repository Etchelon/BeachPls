#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <QObject>

class BeachPlayer;

class Engine : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QList<QObject*> players READ players CONSTANT)

private:
	QList<QObject*> m_players;

public:
	explicit Engine(QObject* parent = nullptr);
	QList<QObject*> players() const;

signals:

public slots:

};

#endif // ENGINE_HPP
