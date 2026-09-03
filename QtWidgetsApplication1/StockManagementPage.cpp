#include "StockManagementPage.h"
#include "ui_StockManagementPage.h"

#include <QMessageBox>
#include <QIntValidator>

StockManagementPage::StockManagementPage(
    InventoryManager* inventoryManager,
    QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::StockManagementPageClass)
    , inventoryManager(inventoryManager)
{
    ui->setupUi(this);

    //connect
    connect(ui->comboBoxProducts, &QComboBox::currentIndexChanged,
        this, &StockManagementPage::updateButtonStates);

    connect(ui->lineEditQuantity, &QLineEdit::textChanged,
        this, &StockManagementPage::updateButtonStates);

    connect(ui->comboBoxTransactionType, &QComboBox::currentIndexChanged,
        this, &StockManagementPage::updateButtonStates);

    connect(ui->comboBoxCustomer, &QComboBox::currentIndexChanged,
        this, &StockManagementPage::updateButtonStates);


    //validator
    ui->lineEditQuantity->setValidator(new QIntValidator(1, 999999, this));

    refresh();
}

StockManagementPage::~StockManagementPage()
{
    delete ui;
}

void StockManagementPage::refresh() {
    loadTransactionTypes();
    loadProducts();
    loadCustomers();
    loadRecentTransactions();
    updateButtonStates();
}

void StockManagementPage::updateButtonStates()
{
    bool hasProductSelected =
        ui->comboBoxProducts->currentData().toInt() != -1;

    bool hasTransactionTypeSelected =
        ui->comboBoxTransactionType->currentData().toInt() != -1;

    bool hasCustomerSelected =
        ui->comboBoxCustomer->currentData().toInt() != -1;

    bool hasTextQuantity = !ui->lineEditQuantity->text().trimmed().isEmpty();

    TransactionType type = static_cast<TransactionType>(
        ui->comboBoxTransactionType->currentData().toInt());

    bool customerRequired =
        type == TransactionType::SALE ||
        type == TransactionType::RETURN_REQUEST ||
        type == TransactionType::RETURN_ACCEPTED ||
        type == TransactionType::RETURN_REJECTED;

    bool customerOk =
        !customerRequired || hasCustomerSelected;

    ui->pushButtonProcessTransaction->setEnabled(hasProductSelected && 
        hasTransactionTypeSelected && hasTextQuantity && customerOk);
}

void StockManagementPage::loadTransactionTypes()
{
    ui->comboBoxTransactionType->clear();

    ui->comboBoxTransactionType->addItem("Select Transaction Type", -1);

    ui->comboBoxTransactionType->addItem(
        "Purchase",
        static_cast<int>(TransactionType::PURCHASE));

    ui->comboBoxTransactionType->addItem(
        "Production",
        static_cast<int>(TransactionType::PRODUCTION));

    ui->comboBoxTransactionType->addItem(
        "Sale",
        static_cast<int>(TransactionType::SALE));

    ui->comboBoxTransactionType->addItem(
        "Usage",
        static_cast<int>(TransactionType::USAGE));

    ui->comboBoxTransactionType->addItem(
        "Loss",
        static_cast<int>(TransactionType::LOSS));

    ui->comboBoxTransactionType->addItem(
        "Return Request",
        static_cast<int>(TransactionType::RETURN_REQUEST));

    ui->comboBoxTransactionType->addItem(
        "Return Accepted",
        static_cast<int>(TransactionType::RETURN_ACCEPTED));

    ui->comboBoxTransactionType->addItem(
        "Return Rejected",
        static_cast<int>(TransactionType::RETURN_REJECTED));
}

void StockManagementPage::loadProducts() {
    ui->comboBoxProducts->clear();

    ui->comboBoxProducts->addItem("Select Product", -1);

    products = inventoryManager->getAllProductsInfoDB();

    for (const ProductInfo& product : products)
    {
        ui->comboBoxProducts->addItem(
            QString::fromStdString(product.productName),
            product.productID);
    }
}

void StockManagementPage::loadCustomers() {
    ui->comboBoxCustomer->clear();

    ui->comboBoxCustomer->addItem("Select Customer", -1);

    vector<GetAllCustomers> customers = inventoryManager->getAllCustomersDB();

    for (const GetAllCustomers& customer : customers)
    {
        ui->comboBoxCustomer->addItem(
            QString::fromStdString(customer.customerFullName),
            customer.customerID);
    }
}

void StockManagementPage::loadRecentTransactions() {

    ui->tableWidgetTransactions->setRowCount(0);
    vector<RecentTransaction> recentTransactions = inventoryManager->getRecentTransactions(0);

    for (const auto& transaction : recentTransactions)
    {
        int row = ui->tableWidgetTransactions->rowCount();

        ui->tableWidgetTransactions->insertRow(row);

        ui->tableWidgetTransactions->setItem(row, 0, new QTableWidgetItem(
            QString::fromStdString(transaction.timeStamp)));

        ui->tableWidgetTransactions->setItem(row, 1, new QTableWidgetItem(
            QString::fromStdString(transaction.productName)));

        ui->tableWidgetTransactions->setItem(row, 2,
            new QTableWidgetItem(
                QString::fromStdString(
                    Transaction::transactionToString(transaction.type))));

        ui->tableWidgetTransactions->setItem(row, 3, new QTableWidgetItem(
            QString::number(transaction.quantity)));

        ui->tableWidgetTransactions->setItem(row, 4, new QTableWidgetItem(
            QString::fromStdString(transaction.customerName)));

        ui->tableWidgetTransactions->setItem(row, 5, new QTableWidgetItem(
            QString::fromStdString(transaction.userName)));
    }
}

void StockManagementPage::on_comboBoxProducts_currentIndexChanged(int index) {
    if (index == 0)
    {
        ui->labelCategory->clear();
        ui->labelCurrentStock->clear();
        ui->labelPrice->clear();
        return;
    }

    if (index - 1 < 0 || index - 1 >= products.size())
        return;

    const ProductInfo& product = products[index - 1];

    ui->labelCategory->setText(
        QString::fromStdString(product.categoryName));

    ui->labelCurrentStock->setText(
        QString::number(product.currentStock));

    ui->labelPrice->setText(
        QString::number(product.price));
}

void StockManagementPage::on_comboBoxTransactionType_currentIndexChanged(int index) {
    TransactionType type = static_cast<TransactionType>(
        ui->comboBoxTransactionType->currentData().toInt());

    bool needsCustomer =
        type == TransactionType::SALE ||
        type == TransactionType::RETURN_REQUEST ||
        type == TransactionType::RETURN_ACCEPTED ||
        type == TransactionType::RETURN_REJECTED;

    ui->comboBoxCustomer->setEnabled(needsCustomer);

    if (!needsCustomer)
        ui->comboBoxCustomer->setCurrentIndex(0);
}

void StockManagementPage::on_pushButtonProcessTransaction_clicked() {

    int productID = ui->comboBoxProducts->currentData().toInt();

    int quantity = ui->lineEditQuantity->text().toInt();

    TransactionType type = static_cast<TransactionType>(
            ui->comboBoxTransactionType->currentData().toInt());

    int customerID = -1;

    switch (type)
    {
    case TransactionType::SALE:
    case TransactionType::RETURN_REQUEST:
    case TransactionType::RETURN_ACCEPTED:
    case TransactionType::RETURN_REJECTED:
        customerID = ui->comboBoxCustomer->currentData().toInt();
        break;

    default:
        customerID = -1;
        break;
    }

    bool success = inventoryManager->processTransactionDB(productID, quantity, type, customerID);

    if (success)
    {
        emit dataChanged();
    }
    else 
    {
        QMessageBox::warning(
            this,
            "Error",
            "Something went wrong, please try again."
        );
        return;
    }
}