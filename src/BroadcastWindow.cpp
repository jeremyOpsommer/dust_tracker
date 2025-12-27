//
// Created for Broadcast View
//
#include <QtWidgets>
#include "headers/BroadcastWindow.h"

BroadcastWindow::BroadcastWindow(PackManager *packManager, QWidget *parent)
    : QWidget(parent), m_packManager(packManager)
{
    setupUI();
    applyBroadcastStyle();
}

BroadcastWindow::~BroadcastWindow() = default;

void BroadcastWindow::setupUI()
{
    setWindowTitle("Broadcast View - Dust Tracker");
    setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
    resize(600, 400);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    // Titre du pack
    m_titleLabel = new QLabel(tr("Items Tracker"));
    QFont titleFont = m_titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    m_titleLabel->setFont(titleFont);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_titleLabel);

    // Grille d'items compacte
    m_itemsGrid = new QGridLayout();
    m_itemsGrid->setSpacing(8);
    mainLayout->addLayout(m_itemsGrid);
    mainLayout->addStretch();

    refreshItems();
}

void BroadcastWindow::applyBroadcastStyle()
{
    // Style sombre optimisé pour OBS/streaming
    setStyleSheet(R"(
        QWidget {
            background-color: #1a1a1a;
            color: #ffffff;
        }
        QLabel {
            color: #ffffff;
        }
        QPushButton {
            border: 2px solid #333;
            border-radius: 6px;
            padding: 5px;
            font-weight: bold;
            font-size: 11pt;
        }
        QPushButton[acquired="true"] {
            background-color: #2e7d32;
            border-color: #4caf50;
        }
        QPushButton[acquired="false"] {
            background-color: #424242;
            border-color: #616161;
            color: #9e9e9e;
        }
    )");
}

void BroadcastWindow::refreshItems()
{
    // Nettoyer la grille
    QLayoutItem *item;
    while ((item = m_itemsGrid->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    if (!m_packManager) return;

    const auto &pack = m_packManager->currentPack();
    
    // Mettre à jour le titre
    if (!pack.name.isEmpty()) {
        m_titleLabel->setText(pack.name + " - Items");
    }

    // Afficher les items en grille compacte (4 colonnes pour broadcast)
    int row = 0, col = 0;
    const int maxCols = 4;

    for (auto it = pack.items.begin(); it != pack.items.end(); ++it) {
        const Item &itm = it.value();
        
        QPushButton *btn = new QPushButton(itm.displayName);
        btn->setFixedSize(120, 70);
        btn->setEnabled(false); // Lecture seule en mode broadcast
        btn->setProperty("acquired", itm.acquired);
        
        m_itemsGrid->addWidget(btn, row, col);

        col++;
        if (col >= maxCols) {
            col = 0;
            row++;
        }
    }
}

void BroadcastWindow::onTrackerUpdated()
{
    refreshItems();
}
