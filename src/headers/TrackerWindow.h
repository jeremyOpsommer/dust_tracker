#pragma once

#include <QMainWindow>
#include <QMenu>
#include <QGridLayout>
#include <QListWidget>
#include <QWidget>
#include "PackManager.h"
#include "LogicEngine.h"

class BroadcastWindow;

class TrackerWindow : public QMainWindow
{
    Q_OBJECT
public:
    TrackerWindow(QWidget *parent = nullptr);
    ~TrackerWindow();

signals:
    void trackerUpdated();

private slots:
    void onItemClicked(const QString &itemId);
    void onLoadPack();
    void onBroadcastView();
    void onResetTracker();

private:
    PackManager m_packManager;
    LogicEngine m_logicEngine;

    // UI Methods
    void setupUI();
    void refreshUI();
    void refreshItemsGrid();
    void refreshLocationsList();
    void createMenus();
    void createToolbar();
    void createCentralWidget();
    QWidget* createItemsPanel();
    QWidget* createLocationsPanel();

    // UI Components
    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;

    QWidget *m_itemsWidget;
    QWidget *m_locationsWidget;
    QGridLayout *m_itemsGrid;
    QListWidget *m_locationsList;

    // Broadcast window
    BroadcastWindow *m_broadcastWindow;
};