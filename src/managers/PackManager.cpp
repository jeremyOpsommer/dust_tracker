#include "PackManager.h"

bool PackManager::loadPack(const QString &filePath)
{
    // TODO: Implémenter le chargement de pack depuis un fichier
    return false;
}

bool PackManager::saveState(const QString &filePath)
{
    // TODO: Implémenter la sauvegarde de l'état
    return false;
}

const GamePack & PackManager::currentPack() const
{
    return m_currentPack;
}

GamePack & PackManager::currentPack()
{
    return m_currentPack;
}
