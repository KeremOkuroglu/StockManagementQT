#pragma once

#include <QWidget>

class InventoryManager;

namespace Ui {
    class DashboardPageClass;
}

class DashboardPage : public QWidget
{
    Q_OBJECT

public:
    explicit DashboardPage(InventoryManager* manager,
        QWidget* parent = nullptr);
    ~DashboardPage();

    void refresh();

    void loadWelcomeMessage();

    void loadProductCount();
    void loadCategoryCount();
    void loadCustomerCount();

    void loadTotalStockValue();
    void loadLowStockCount();
    void loadTodayOrderCount();

    void loadRecentTransactions();
    void loadLowStockProducts();

private:
    Ui::DashboardPageClass* ui;

    InventoryManager* inventoryManager;
};