#include "ReportsPage.h"
#include "ui_ReportsPage.h"
#include "qmessagebox.h"

ReportsPage::ReportsPage(
    InventoryManager* manager,
    QWidget* parent)
    : QWidget(parent),
    ui(new Ui::ReportsPageClass),
    inventoryManager(manager)
{
    ui->setupUi(this);

    refresh();
}

ReportsPage::~ReportsPage()
{
    delete ui;
}

void ReportsPage::refresh() {
    loadReportTypes();
    loadTotalProducts();
    loadTotalCustomers();
    loadTotalTransactions();
}

void ReportsPage::setupTable(const QStringList& headers)
{
    ui->tableWidgetReports->clear();
    ui->tableWidgetReports->setRowCount(0);

    ui->tableWidgetReports->setColumnCount(headers.size());
    ui->tableWidgetReports->setHorizontalHeaderLabels(headers);

    ui->tableWidgetReports->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidgetReports->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableWidgetReports->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetReports->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetReports->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->tableWidgetReports->verticalHeader()->setVisible(false);
}

void ReportsPage::on_comboBoxReports_currentIndexChanged(int index)
{
    switch (index)
    {
    case 0:
        loadInventoryReport();
        break;

    case 1:
        loadTransactionReport();
        break;

    case 2:
        loadProductReport();
        break;

    case 3:
        loadCustomerReport();
        break;
    }
}

void ReportsPage::loadReportTypes() {
    ui->comboBoxReports->clear();

    ui->comboBoxReports->addItem("Inventory Summary");
    ui->comboBoxReports->addItem("Transaction Report");
    ui->comboBoxReports->addItem("Product Report");
    ui->comboBoxReports->addItem("Customer Report");
}

void ReportsPage::loadTotalProducts() {
    int productCount = inventoryManager->getProductCount();

    ui->labelTotalProducts->setText(QString::number(productCount));
}

void ReportsPage::loadTotalCustomers() {
    int customerCount = inventoryManager->getCustomerCount();

    ui->labelTotalCustomers->setText(QString::number(customerCount));
}

void ReportsPage::loadTotalTransactions() {
    int totalTransactionCount = inventoryManager->getTransactionCount();

    ui->labelTotalTransactions->setText(QString::number(totalTransactionCount));
}

void ReportsPage::loadInventoryReport() {
    setupTable({
        "ID",
        "Product",
        "Category",
        "Current",
        "Price",
        "Total Value"
        });

    ui->tableWidgetReports->setRowCount(0);

    vector<InventorySummaryReport> inventorySummaryReport = inventoryManager->getAllInventorySummaryReport();

    for (const auto& report : inventorySummaryReport)
    {
        int row = ui->tableWidgetReports->rowCount();

        ui->tableWidgetReports->insertRow(row);

        ui->tableWidgetReports->setItem(row, 0, new QTableWidgetItem(
            QString::number(report.productID)));

        ui->tableWidgetReports->setItem(row, 1, new QTableWidgetItem(
            QString::fromStdString(report.productName)));

        ui->tableWidgetReports->setItem(row, 2, new QTableWidgetItem(
            QString::fromStdString(report.categoryName)));

        ui->tableWidgetReports->setItem(row, 3, new QTableWidgetItem(
            QString::number(report.currentStock)));


        ui->tableWidgetReports->setItem(row, 4, new QTableWidgetItem(
            QString::number(report.unitPrice)));

        ui->tableWidgetReports->setItem(row, 5, new QTableWidgetItem(
            QString::number(report.totalValue)));
    }
}

void ReportsPage::loadTransactionReport() {

    setupTable({
        "ID",
        "Product",
        "Customer",
        "User",
        "Type",
        "Quantity",
        "Date"
        });
    ui->tableWidgetReports->setRowCount(0);


    vector<TransactionReport> transactionReports = inventoryManager->getAllTransactionReport();


    for (const auto& report : transactionReports)
    {
        int row = ui->tableWidgetReports->rowCount();

        ui->tableWidgetReports->insertRow(row);

        ui->tableWidgetReports->setItem(row, 0, new QTableWidgetItem(
            QString::number(report.transactionID)));

        ui->tableWidgetReports->setItem(row, 1, new QTableWidgetItem(
            QString::fromStdString(report.productName)));

        ui->tableWidgetReports->setItem(row, 2, new QTableWidgetItem(
            QString::fromStdString(report.customerName)));

        ui->tableWidgetReports->setItem(row, 3, new QTableWidgetItem(
            QString::fromStdString(report.userName)));

        ui->tableWidgetReports->setItem(row, 4, new QTableWidgetItem(
            QString::fromStdString(report.transactionType)));

        ui->tableWidgetReports->setItem(row, 5, new QTableWidgetItem(
            QString::number(report.quantity)));

        ui->tableWidgetReports->setItem(row, 6, new QTableWidgetItem(
            QString::fromStdString(report.transactionDate)));
    }
}

void ReportsPage::loadProductReport() {
    setupTable({
    "ID",
    "Product",
    "Purchased",
    "Produced",
    "Sold",
    "Used",
    "Returned",
    "Lost",
    "Current"
        });

    ui->tableWidgetReports->setRowCount(0);


    vector<ProductReport> productReports = inventoryManager->getAllProductReport();


    for (const auto& report : productReports)
    {
        int row = ui->tableWidgetReports->rowCount();

        ui->tableWidgetReports->insertRow(row);

        ui->tableWidgetReports->setItem(row, 0, new QTableWidgetItem(
            QString::number(report.productID)));

        ui->tableWidgetReports->setItem(row, 1, new QTableWidgetItem(
            QString::fromStdString(report.productName)));

        ui->tableWidgetReports->setItem(row, 2, new QTableWidgetItem(
            QString::number(report.purchased)));

        ui->tableWidgetReports->setItem(row, 3, new QTableWidgetItem(
            QString::number(report.produced)));

        ui->tableWidgetReports->setItem(row, 4, new QTableWidgetItem(
            QString::number(report.sold)));

        ui->tableWidgetReports->setItem(row, 5, new QTableWidgetItem(
            QString::number(report.used)));

        ui->tableWidgetReports->setItem(row, 6, new QTableWidgetItem(
            QString::number(report.returned)));

        ui->tableWidgetReports->setItem(row, 7, new QTableWidgetItem(
            QString::number(report.lost)));

        ui->tableWidgetReports->setItem(row, 8, new QTableWidgetItem(
            QString::number(report.currentStock)));
    }
}

void ReportsPage::loadCustomerReport() {
    setupTable({
    "ID",
    "Customer",
    "Purchases",
    "Returns",
    "Last Transaction"
        });
    ui->tableWidgetReports->setRowCount(0);


    vector<CustomerReport> customerReports = inventoryManager->getAllCustomerReport();


    for (const auto& report : customerReports)
    {
        int row = ui->tableWidgetReports->rowCount();

        ui->tableWidgetReports->insertRow(row);

        ui->tableWidgetReports->setItem(row, 0, new QTableWidgetItem(
            QString::number(report.customerID)));

        ui->tableWidgetReports->setItem(row, 1, new QTableWidgetItem(
            QString::fromStdString(report.customerName)));

        ui->tableWidgetReports->setItem(row, 2, new QTableWidgetItem(
            QString::number(report.totalPurchases)));

        ui->tableWidgetReports->setItem(row, 3, new QTableWidgetItem(
            QString::number(report.totalReturns)));

        ui->tableWidgetReports->setItem(row, 4, new QTableWidgetItem(
            QString::fromStdString(report.lastTransactionDate)));
    }
}