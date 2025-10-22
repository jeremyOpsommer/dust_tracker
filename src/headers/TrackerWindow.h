#pragma once

#include <QMainWindow>
#include "PackManager.h"
#include "LogicEngine.h"

class TrackerWindow : public QMainWindow
{
    Q_OBJECT
public:
    TrackerWindow(QWidget *parent = nullptr);
    ~TrackerWindow();

private slots:
    void onItemClicked(const QString &itemId);
    void onLoadPack();
    void onBroadcastView();

private:
    PackManager m_packManager;
    LogicEngine m_logicEngine;

    // UI composants : menu, panneau d’items, panneau de map / locations, etc.
    void setupUI();
    void refreshUI();
};