#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "InventoryManager.h"
class DashboardPage;
class Categories;
class ProductPage;
class CustomersPage;
class TransactionsPage;
class ReportsPage;
class UsersPage;
class StockManagementPage;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(InventoryManager *manager, QWidget *parent = nullptr);
    ~MainWindow() override;

    bool isLogout = false;

    void refreshAllPages();

private slots:
    void on_listWidget_currentRowChanged(int currentRow);

private:
    void logout();

    void updateMenuPermissions();

    int previousPage = 0;

    Ui::MainWindow *ui;

    InventoryManager *inventoryManager;

    DashboardPage* dashboardPage;

    Categories* categoriesPage;

    ProductPage* productPage;

    CustomersPage* customersPage;

    TransactionsPage* transactionPage;

    ReportsPage* reportsPage;

    UsersPage* usersPage;

    StockManagementPage* stockManagementPage;
};
#endif // MAINWINDOW_H
