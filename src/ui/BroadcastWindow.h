#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include "../managers/PackManager.h"

class BroadcastWindow : public QWidget
{
    Q_OBJECT
public:
    explicit BroadcastWindow(PackManager *packManager, QWidget *parent = nullptr);
    ~BroadcastWindow();

public slots:
    void onTrackerUpdated();

private:
    void setupUI();
    void refreshItems();
    void applyBroadcastStyle();

    PackManager *m_packManager;
    QGridLayout *m_itemsGrid;
    QLabel *m_titleLabel;
};
