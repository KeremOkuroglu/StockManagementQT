#include "TransactionsPage.h"
#include "ui_TransactionsPage.h"
#include "InventoryManager.h"

TransactionsPage::TransactionsPage(
    InventoryManager* manager,
    QWidget* parent)
    : QWidget(parent),
    ui(new Ui::TransactionsPageClass),
    inventoryManager(manager)
{
    ui->setupUi(this);

    connect(ui->lineEditSearch, &QLineEdit::textChanged,
        this, &TransactionsPage::updateButtonStates);

    //search with enter
    connect(ui->lineEditSearch,
        &QLineEdit::returnPressed,
        this,
        &TransactionsPage::on_pushButtonSearch_clicked);

    //search without press enter or search, just change comboBox
    connect(ui->comboBoxType,
        &QComboBox::currentIndexChanged,
        this,
        &TransactionsPage::on_pushButtonSearch_clicked);

    //
    connect(ui->lineEditSearch,
        &QLineEdit::textChanged,
        this,
        [this](const QString& text)
        {
            if (text.trimmed().isEmpty()
                && ui->comboBoxType->currentData().toInt() == -1)
            {
                loadTransactions();
            }
        });

    refresh();
}

TransactionsPage::~TransactionsPage()
{
    delete ui;
}

void TransactionsPage::refresh()
{
    loadTransactions();
    loadTransactionTypes();
    updateButtonStates();
}

void TransactionsPage::updateButtonStates()
{
    bool hasText = !ui->lineEditSearch->text().trimmed().isEmpty();

    ui->pushButtonSearch->setEnabled(hasText);
}

void TransactionsPage::loadTransactionTypes()
{
    ui->comboBoxType->clear();

    ui->comboBoxType->addItem(
        "All", -1);

    ui->comboBoxType->addItem(
        "Purchase",
        static_cast<int>(TransactionType::PURCHASE));

    ui->comboBoxType->addItem(
        "Production",
        static_cast<int>(TransactionType::PRODUCTION));

    ui->comboBoxType->addItem(
        "Sale",
        static_cast<int>(TransactionType::SALE));

    ui->comboBoxType->addItem(
        "Usage",
        static_cast<int>(TransactionType::USAGE));

    ui->comboBoxType->addItem(
        "Loss",
        static_cast<int>(TransactionType::LOSS));

    ui->comboBoxType->addItem(
        "Return Request",
        static_cast<int>(TransactionType::RETURN_REQUEST));

    ui->comboBoxType->addItem(
        "Return Accepted",
        static_cast<int>(TransactionType::RETURN_ACCEPTED));

    ui->comboBoxType->addItem(
        "Return Rejected",
        static_cast<int>(TransactionType::RETURN_REJECTED));
}


void TransactionsPage::loadTransactions()
{
    ui->tableWidgetTransactions->setRowCount(0);
    vector<RecentTransaction> recentTransactions = inventoryManager->getRecentTransactions(0);

    for (const auto& transaction : recentTransactions)
    {
        int row = ui->tableWidgetTransactions->rowCount();

        ui->tableWidgetTransactions->insertRow(row);

        ui->tableWidgetTransactions->setItem(row, 0, new QTableWidgetItem(
            QString::number(transaction.transactionID)));

        ui->tableWidgetTransactions->setItem(row, 1, new QTableWidgetItem(
            QString::fromStdString(transaction.productName)));

        ui->tableWidgetTransactions->setItem(row, 2, new QTableWidgetItem(
            QString::number(transaction.userID)));

        ui->tableWidgetTransactions->setItem(row, 3, new QTableWidgetItem(
            QString::fromStdString(
                Transaction::transactionToString(transaction.type))));

        ui->tableWidgetTransactions->setItem(row, 4, new QTableWidgetItem(
            QString::number(transaction.quantity)));

        ui->tableWidgetTransactions->setItem(row, 5, new QTableWidgetItem(
            QString::fromStdString(transaction.timeStamp)));
    }
}

void TransactionsPage::on_pushButtonSearch_clicked()
{
    ui->tableWidgetTransactions->setRowCount(0);

    string keyword = ui->lineEditSearch->text().toStdString();

    int typeFilter = ui->comboBoxType->currentData().toInt();

    string type = "";

    if (typeFilter != -1)
    {
        type = Transaction::transactionToString(
            static_cast<TransactionType>(typeFilter));
    }

    vector<RecentTransaction> transactions =
        inventoryManager->searchTransactions(keyword, type);

    for (const auto& transaction : transactions)
    {
        int row = ui->tableWidgetTransactions->rowCount();

        ui->tableWidgetTransactions->insertRow(row);

        ui->tableWidgetTransactions->setItem(row, 0,
            new QTableWidgetItem(QString::number(transaction.transactionID)));

        ui->tableWidgetTransactions->setItem(row, 1,
            new QTableWidgetItem(QString::fromStdString(transaction.productName)));

        ui->tableWidgetTransactions->setItem(row, 2,
            new QTableWidgetItem(QString::number(transaction.userID)));

        ui->tableWidgetTransactions->setItem(row, 3,
            new QTableWidgetItem(QString::fromStdString(
                Transaction::transactionToString(transaction.type))));

        ui->tableWidgetTransactions->setItem(row, 4,
            new QTableWidgetItem(QString::number(transaction.quantity)));

        ui->tableWidgetTransactions->setItem(row, 5,
            new QTableWidgetItem(QString::fromStdString(transaction.timeStamp)));
    }
}