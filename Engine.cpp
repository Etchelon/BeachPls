#include "Engine.hpp"
#include "BeachPlayer.hpp"

Engine::Engine(QObject* parent)
	: QObject{ parent }
{
	m_players.append(new BeachPlayer{ "Fish", 1.74 });
	m_players.append(new BeachPlayer{ "Vale", 1.65 });
	m_players.append(new BeachPlayer{ "Luca", 1.96 });
	m_players.append(new BeachPlayer{ "Phil", 1.70 });
	m_players.append(new BeachPlayer{ "Bad", 1.72 });
	m_players.append(new BeachPlayer{ "Peppo", 1.72 });
	m_players.append(new BeachPlayer{ "Roby", 1.74 });
	m_players.append(new BeachPlayer{ "Zilli", 1.75 });
	m_players.append(new BeachPlayer{ "Kuji", 1.78 });
	m_players.append(new BeachPlayer{ "Ila", 1.65 });
	m_players.append(new BeachPlayer{ "Mari", 1.78 });
	m_players.append(new BeachPlayer{ "Giulia", 1.74 });
	m_players.append(new BeachPlayer{ "Regia", 1.80 });
	m_players.append(new BeachPlayer{ "Alby", 1.82 });
	m_players.append(new BeachPlayer{ "Silvia", 1.68 });
	m_players.append(new BeachPlayer{ "Mema", 1.70 });
	m_players.append(new BeachPlayer{ "Moni", 1.65 });
	m_players.append(new BeachPlayer{ "Dado", 1.76 });
	m_players.append(new BeachPlayer{ "Fabbro", 1.70 });
}

QList<QObject*> Engine::players() const
{
	return m_players;
}
