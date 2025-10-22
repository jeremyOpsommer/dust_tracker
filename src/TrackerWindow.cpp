//
// Created by jerem on 06/10/2025.
//
#include "headers/TrackerWindow.h"

TrackerWindow::TrackerWindow(QWidget *parent) : QMainWindow(parent)
{
    setupUI();
}

TrackerWindow::~TrackerWindow() = default;

void TrackerWindow::setupUI()
{
    setWindowTitle("Dust Tracker");
    resize(800, 600);

    // TODO: Implémenter l'interface utilisateur complète
}

void TrackerWindow::refreshUI()
{
    // TODO: Rafraîchir l'interface
}

void TrackerWindow::onItemClicked(const QString &itemId)
{
    auto &pack = m_packManager.currentPack();
    if (pack.items.contains(itemId)) {
        pack.items[itemId].acquired = !pack.items[itemId].acquired;
        // Recalculer la logique / visibilité
        m_logicEngine.updateVisibility(pack);
        // Mettre à jour l'interface
        refreshUI();
    }
}

void TrackerWindow::onLoadPack()
{
    // TODO: Implémenter le chargement de pack
}

void TrackerWindow::onBroadcastView()
{
    // TODO: Implémenter la vue broadcast
}