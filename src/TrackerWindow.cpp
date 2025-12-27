//
// Created by jerem on 06/10/2025.
//
#include <QtWidgets>
#include "headers/TrackerWindow.h"
#include "headers/BroadcastWindow.h"

TrackerWindow::TrackerWindow(QWidget *parent) : QMainWindow(parent), m_broadcastWindow(nullptr)
{
    setupUI();
}

TrackerWindow::~TrackerWindow()
{
    if (m_broadcastWindow) {
        delete m_broadcastWindow;
    }
}

void TrackerWindow::setupUI()
{
    setWindowTitle("Dust Tracker");
    resize(1200, 800);

    createMenus();
    createToolbar();
    createCentralWidget();
}

void TrackerWindow::refreshUI()
{
    refreshItemsGrid();
    refreshLocationsList();
}

void TrackerWindow::refreshItemsGrid() {
    // Nettoyer la grille existante
    QLayoutItem *item;
    while ((item = m_itemsGrid->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // Remplir avec les items du pack actuel
    const auto &pack = m_packManager.currentPack();
    int row = 0, col = 0;
    const int maxCols = 6;

    for (auto it = pack.items.begin(); it != pack.items.end(); ++it) {
        const Item &item = it.value();
        QPushButton *btn = new QPushButton(item.displayName);
        btn->setFixedSize(80, 80);
        btn->setCheckable(true);
        btn->setChecked(item.acquired);

        // Style visuel
        QString style = item.acquired
            ? "background-color: #4CAF50; color: white; font-weight: bold;"
            : "background-color: #757575; color: #ddd;";
        btn->setStyleSheet(style);

        connect(btn, &QPushButton::clicked, this, [this, id = item.id]() {
            onItemClicked(id);
        });

        m_itemsGrid->addWidget(btn, row, col);

        col++;
        if (col >= maxCols) {
            col = 0;
            row++;
        }
    }
}

void TrackerWindow::refreshLocationsList() {
    m_locationsList->clear();

    const auto &pack = m_packManager.currentPack();
    for (auto it = pack.locations.begin(); it != pack.locations.end(); ++it) {
        const Location &loc = it.value();

        QListWidgetItem *listItem = new QListWidgetItem(loc.displayName);

        // Couleur selon statut
        if (loc.visited) {
            listItem->setForeground(Qt::green);
        } else {
            listItem->setForeground(Qt::gray);
        }

        m_locationsList->addItem(listItem);
    }
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
        emit trackerUpdated();
    }
}

void TrackerWindow::onLoadPack()
{
    QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("Charger un Pack"),
        QString(),
        tr("Game Packs (*.json)")
    );

    if (!filePath.isEmpty()) {
        if (m_packManager.loadPack(filePath)) {
            m_logicEngine.updateVisibility(m_packManager.currentPack());
            refreshUI();
            statusBar()->showMessage(tr("Pack chargé avec succès"), 3000);
        } else {
            QMessageBox::warning(this, tr("Erreur"), tr("Impossible de charger le pack"));
        }
    }
}

void TrackerWindow::onResetTracker()
{
    auto reply = QMessageBox::question(
        this,
        tr("Réinitialiser"),
        tr("Voulez-vous vraiment réinitialiser tous les items ?"),
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        auto &pack = m_packManager.currentPack();
        for (auto &item : pack.items) {
            item.acquired = false;
        }
        for (auto &loc : pack.locations) {
            loc.visited = false;
        }
        m_logicEngine.updateVisibility(pack);
        refreshUI();
        emit trackerUpdated();
    }
}

void TrackerWindow::onBroadcastView()
{
    if (!m_broadcastWindow) {
        m_broadcastWindow = new BroadcastWindow(&m_packManager, this);
        connect(this, &TrackerWindow::trackerUpdated, m_broadcastWindow, &BroadcastWindow::onTrackerUpdated);
    }
    m_broadcastWindow->show();
    m_broadcastWindow->raise();
    m_broadcastWindow->activateWindow();
}

void TrackerWindow::createMenus() {
    fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *loadPackAction = fileMenu->addAction(tr("&Charger Pack..."));
    connect(loadPackAction, &QAction::triggered, this, &TrackerWindow::onLoadPack);

    QAction *saveStateAction = fileMenu->addAction(tr("&Sauvegarder État"));
    QAction *loadStateAction = fileMenu->addAction(tr("&Charger État"));
    fileMenu->addSeparator();

    QAction *quitAction = fileMenu->addAction(tr("&Quitter"));
    connect(quitAction, &QAction::triggered, this, &QWidget::close);

    viewMenu = menuBar()->addMenu(tr("&Affichage"));
    QAction *broadcastAction = viewMenu->addAction(tr("Mode &Broadcast"));
    connect(broadcastAction, &QAction::triggered, this, &TrackerWindow::onBroadcastView);

    helpMenu = menuBar()->addMenu(tr("&Aide"));
    QAction *aboutAction = helpMenu->addAction(tr("À &propos"));
}

void TrackerWindow::createToolbar() {
    QToolBar *toolbar = addToolBar(tr("Principale"));
    toolbar->setMovable(false);

    QAction *resetAction = toolbar->addAction(tr("Réinitialiser"));
    connect(resetAction, &QAction::triggered, this, &TrackerWindow::onResetTracker);

    toolbar->addSeparator();

    QAction *undoAction = toolbar->addAction(tr("Annuler"));
    QAction *redoAction = toolbar->addAction(tr("Refaire"));
}

void TrackerWindow::createCentralWidget() {
    // Widget central avec splitter
    QSplitter *mainSplitter = new QSplitter(Qt::Horizontal, this);

    // Panneau de gauche : Grille d'items
    m_itemsWidget = createItemsPanel();
    mainSplitter->addWidget(m_itemsWidget);

    // Panneau de droite : Locations
    m_locationsWidget = createLocationsPanel();
    mainSplitter->addWidget(m_locationsWidget);

    // Ratio 60/40
    mainSplitter->setStretchFactor(0, 6);
    mainSplitter->setStretchFactor(1, 4);

    setCentralWidget(mainSplitter);
}

QWidget* TrackerWindow::createItemsPanel() {
    QWidget *panel = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(panel);

    QLabel *title = new QLabel(tr("Items"));
    QFont titleFont = title->font();
    titleFont.setPointSize(12);
    titleFont.setBold(true);
    title->setFont(titleFont);
    layout->addWidget(title);

    // Grille d'items (sera remplie dynamiquement)
    m_itemsGrid = new QGridLayout();
    m_itemsGrid->setSpacing(5);
    layout->addLayout(m_itemsGrid);
    layout->addStretch();

    return panel;
}

QWidget* TrackerWindow::createLocationsPanel() {
    QWidget *panel = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(panel);

    QLabel *title = new QLabel(tr("Locations"));
    QFont titleFont = title->font();
    titleFont.setPointSize(12);
    titleFont.setBold(true);
    title->setFont(titleFont);
    layout->addWidget(title);

    // Liste scrollable de locations
    m_locationsList = new QListWidget();
    layout->addWidget(m_locationsList);

    return panel;
}