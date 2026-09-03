#include "DashboardPage.h"
#include "ui_DashboardPage.h"
#include "InventoryManager.h"

DashboardPage::DashboardPage(
    InventoryManager* manager,
    QWidget* parent)
    : QWidget(parent),
    ui(new Ui::DashboardPageClass),
    inventoryManager(manager)
{
    ui->setupUi(this);

    refresh();
}

DashboardPage::~DashboardPage()
{
    delete ui;
}

void DashboardPage::refresh()
{
    loadWelcomeMessage();

    loadProductCount();
    loadCategoryCount();
    loadCustomerCount();

    loadTotalStockValue();
    loadLowStockCount();
    loadTodayOrderCount();

    loadRecentTransactions();
    loadLowStockProducts();
}

void DashboardPage::loadWelcomeMessage()
{
    ui->labelWelcome->setText(
        "Welcome, " + 
        QString::fromStdString(inventoryManager->getCurrentUser()->getUserName()));
}

void DashboardPage::loadProductCount() {
    int productCount = inventoryManager->getProductCount();

    ui->labelProductCount->setText(QString::number(productCount));
}

void DashboardPage::loadCategoryCount() {
    int categoryCount = inventoryManager->getCategoryCount();

    ui->labelCategoryCount->setText(QString::number(categoryCount));
}

void DashboardPage::loadCustomerCount() {
    int customerCount = inventoryManager->getCustomerCount();

    ui->labelCustomerCount->setText(QString::number(customerCount));
}



void DashboardPage::loadTotalStockValue() {
    int totalStockValue = inventoryManager->getTotalStockValue();

    ui->labelTotalStockValueCount->setText(QString::number(totalStockValue));
}

void DashboardPage::loadLowStockCount() {
    int lowStockCount = inventoryManager->getLowStockCount();

    ui->labelLowStockCount->setText(QString::number(lowStockCount));
}

void DashboardPage::loadTodayOrderCount() {
    int todayOrderCount = inventoryManager->getTodayOrderCount();

    ui->labelTodayOrdersCount->setText(QString::number(todayOrderCount));
}



void DashboardPage::loadRecentTransactions() {
    ui->tableRecentTransactions->setRowCount(0);
    vector<RecentTransaction> recentTransactions = inventoryManager->getRecentTransactions();

    for (const auto& transaction : recentTransactions)
    {
        int row = ui->tableRecentTransactions->rowCount();

        ui->tableRecentTransactions->insertRow(row);

        ui->tableRecentTransactions->setItem(row, 0, new QTableWidgetItem(
                QString::fromStdString(transaction.timeStamp)));

        ui->tableRecentTransactions->setItem(row, 1, new QTableWidgetItem(
                QString::fromStdString(transaction.productName)));

        ui->tableRecentTransactions->setItem(row, 2,
            new QTableWidgetItem(
                QString::fromStdString(
                    Transaction::transactionToString(transaction.type))));

        ui->tableRecentTransactions->setItem(row, 3, new QTableWidgetItem(
            QString::number(transaction.quantity)));
    }
}

void DashboardPage::loadLowStockProducts() {
    ui->tableLowStock->setRowCount(0);

    vector<LowStockProduct> products = inventoryManager->getLowStockProducts();

    for (const auto& product : products)
    {
        int row = ui->tableLowStock->rowCount();

        ui->tableLowStock->insertRow(row);

        ui->tableLowStock->setItem(row, 0, new QTableWidgetItem(
            QString::fromStdString(product.productName)));

        ui->tableLowStock->setItem(row, 1, new QTableWidgetItem(
            QString::number(product.currentStock)));

        ui->tableLowStock->setItem(row, 2, new QTableWidgetItem(
            QString::number(product.price)));
    }
}