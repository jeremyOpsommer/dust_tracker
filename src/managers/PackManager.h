#pragma once

#include <QString>
#include <QJsonObject>
#include <QMap>

struct Item {
    QString id;
    QString displayName;
    bool acquired = false;
    // autres propriétés
};

struct Location {
    QString id;
    QString displayName;
    bool visited = false;
    // autres propriétés
};

struct GamePack {
    QString name;
    QString description;
    QMap<QString, Item> items;
    QMap<QString, Location> locations;
    QJsonObject layout;  // JSON décrivant positions, tailles, etc.
    QJsonObject logic;   // JSON décrivant les règles
};

class PackManager {
public:
    bool loadPack(const QString &filePath);
    bool saveState(const QString &filePath);
    // accesseurs
    const GamePack & currentPack() const;
    GamePack & currentPack();

private:
    GamePack m_currentPack;
};