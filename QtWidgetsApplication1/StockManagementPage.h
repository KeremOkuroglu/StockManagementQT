#pragma once

#include <QWidget>

#include "InventoryManager.h"

namespace Ui
{
    class StockManagementPageClass;
}

class StockManagementPage : public QWidget
{
    Q_OBJECT

public:
    explicit StockManagementPage(InventoryManager* inventoryManager,
        QWidget* parent = nullptr);
    ~StockManagementPage();

    void refresh();

signals:
    void dataChanged();

private slots:

    void on_comboBoxProducts_currentIndexChanged(int index);

    void on_comboBoxTransactionType_currentIndexChanged(int index);

    void on_pushButtonProcessTransaction_clicked();

private:

    void loadProducts();

    void loadCustomers();

    void loadRecentTransactions();

    void loadTransactionTypes();

    void updateButtonStates();

private:

    Ui::StockManagementPageClass* ui;

    InventoryManager* inventoryManager;

    vector<ProductInfo> products;
};