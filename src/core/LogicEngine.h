#pragma once

#include "../managers/PackManager.h"

class LogicEngine {
public:
    // Évalue une condition logique, ex: "itemA && (itemB || itemC)"
    bool evaluateCondition(const QJsonObject &condition, const GamePack &pack);
    // Détermine quelles locations sont visibles/acessibles selon la logique
    void updateVisibility(GamePack &pack);
};