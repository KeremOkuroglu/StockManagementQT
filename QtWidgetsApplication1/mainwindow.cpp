#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "logindialog.h"

#include "DashboardPage.h"
#include "ProductPage.h"
#include "CustomersPage.h"
#include "TransactionsPage.h"
#include "ReportsPage.h"
#include "Categories.h"
#include "UsersPage.h"
#include "StockManagementPage.h"

#include <QVBoxLayout>
#include <QMessageBox.h>

MainWindow::MainWindow(InventoryManager *manager, QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    inventoryManager(manager)
{

    ui->setupUi(this);

    dashboardPage = new DashboardPage(inventoryManager);
    
    categoriesPage = new Categories(inventoryManager);

    productPage = new ProductPage(inventoryManager);

    customersPage = new CustomersPage(inventoryManager);

    transactionPage = new TransactionsPage(inventoryManager);

    reportsPage = new ReportsPage(inventoryManager);

    usersPage = new UsersPage(inventoryManager);

    stockManagementPage = new StockManagementPage(inventoryManager);

    QVBoxLayout* dashboardLayout = new QVBoxLayout(ui->page);
    dashboardLayout->setContentsMargins(0, 0, 0, 0);
    dashboardLayout->addWidget(dashboardPage);


    QVBoxLayout* categoryLayout = new QVBoxLayout(ui->page_2);
    categoryLayout->setContentsMargins(0, 0, 0, 0);
    categoryLayout->addWidget(categoriesPage);


    QVBoxLayout* productLayout = new QVBoxLayout(ui->page_3);
    productLayout->setContentsMargins(0, 0, 0, 0);
    productLayout->addWidget(productPage);


    QVBoxLayout* customersPageLayout = new QVBoxLayout(ui->page_4);
    customersPageLayout->setContentsMargins(0, 0, 0, 0);
    customersPageLayout->addWidget(customersPage);


    QVBoxLayout* transactionPageLayout = new QVBoxLayout(ui->page_5);
    transactionPageLayout->setContentsMargins(0, 0, 0, 0);
    transactionPageLayout->addWidget(transactionPage);


    QVBoxLayout* reportsLayout = new QVBoxLayout(ui->page_6);
    reportsLayout->setContentsMargins(0, 0, 0, 0);
    reportsLayout->addWidget(reportsPage);


    QVBoxLayout* usersPageLayout = new QVBoxLayout(ui->page_7);
    usersPageLayout->setContentsMargins(0, 0, 0, 0);
    usersPageLayout->addWidget(usersPage);

    QVBoxLayout* stockManagementLayout = new QVBoxLayout(ui->page_8);
    stockManagementLayout->setContentsMargins(0, 0, 0, 0);
    stockManagementLayout->addWidget(stockManagementPage);


    //Connect for refreshAllPages

    connect(productPage,
        &ProductPage::dataChanged,
        this,
        &MainWindow::refreshAllPages);

    connect(customersPage,
        &CustomersPage::dataChanged,
        this,
        &MainWindow::refreshAllPages);

    connect(categoriesPage,
        &Categories::dataChanged,
        this,
        &MainWindow::refreshAllPages);

    connect(transactionPage,
        &TransactionsPage::dataChanged,
        this,
        &MainWindow::refreshAllPages);

    connect(usersPage,
        &UsersPage::dataChanged,
        this,
        &MainWindow::refreshAllPages);

    connect(stockManagementPage,
        &StockManagementPage::dataChanged,
        this,
        &MainWindow::refreshAllPages);

    refreshAllPages();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refreshAllPages() {
    categoriesPage->refresh();
    dashboardPage->refresh();
    categoriesPage->refresh();
    productPage->refresh();
    customersPage->refresh();
    transactionPage->refresh();
    reportsPage->refresh();
    usersPage->refresh();
    stockManagementPage->refresh();

    updateMenuPermissions();
}

void MainWindow::updateMenuPermissions()
{
    QListWidgetItem* usersItem = ui->listWidget->item(6);

    if (inventoryManager->isAdmin())
    {
        usersItem->setFlags(usersItem->flags() | Qt::ItemIsEnabled);
    }
    else
    {
        usersItem->setFlags(usersItem->flags() & ~Qt::ItemIsEnabled);
    }
}


void MainWindow::on_listWidget_currentRowChanged(int currentRow)
{
    if (currentRow == ui->listWidget->count() - 1)
    {
        logout();
        return;
    }

    previousPage = currentRow;

    ui->stackedWidget->setCurrentIndex(currentRow);
}

void MainWindow::logout()
{
    auto reply = QMessageBox::question(
        this,
        "Logout",
        "Are you sure you want to logout?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes)
    {
        ui->listWidget->setCurrentRow(previousPage);
        return;
    }

    hide();

    inventoryManager->logout();

    LoginDialog login(inventoryManager);

    if (login.exec() == QDialog::Accepted)
    {
        show();

        ui->listWidget->setCurrentRow(0);

        refreshAllPages();
    }
    else
    {
        close();
    }
}