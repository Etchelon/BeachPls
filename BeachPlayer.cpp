#include "BeachPlayer.hpp"

BeachPlayer::BeachPlayer(QObject* parent)
	: QObject{ parent }
{
}

BeachPlayer::BeachPlayer(QString name, double height)
	: m_name{ name }, m_height{ height },
	  m_attack{ 1 },
	  m_pass{ 1 },
	  m_serve{ 1 },
	  m_set{ 1 },
	  m_reaction{ 1 },
	  m_discipline{ 1 }
{
}
