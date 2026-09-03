#include "CustomersPage.h"
#include "ui_CustomersPage.h"
#include "InventoryManager.h"

#include <QRegularExpressionValidator>

#include "qmessagebox.h"

CustomersPage::CustomersPage(
    InventoryManager* manager,
    QWidget* parent)
    : QWidget(parent),
    ui(new Ui::CustomersPageClass),
    inventoryManager(manager)
{
    ui->setupUi(this);

    connect(ui->lineEditAddCustomerName, &QLineEdit::textChanged,
        this, &CustomersPage::updateButtonStates);

    connect(ui->lineEditAddCustomerEmail, &QLineEdit::textChanged,
        this, &CustomersPage::updateButtonStates);


    //phone
    connect(ui->lineEditAddCustomerPhone, &QLineEdit::textChanged,
        this, &CustomersPage::updateButtonStates);

    ui->lineEditAddCustomerPhone->setMaxLength(11);

    QRegularExpression regex("\\d{0,11}");
    ui->lineEditAddCustomerPhone->setValidator(
        new QRegularExpressionValidator(regex, this));

    //

    connect(ui->tableWidgetCustomers, &QTableWidget::itemSelectionChanged,
        this, &CustomersPage::updateButtonStates);

    connect(ui->lineEditSearch, &QLineEdit::textChanged,
        this, &CustomersPage::updateButtonStates);

    //search with enter
    connect(ui->lineEditSearch,
        &QLineEdit::returnPressed,
        this,
        &CustomersPage::on_pushButtonSearch_clicked);

    //search connect
    connect(ui->lineEditSearch,
        &QLineEdit::textChanged,
        this,
        [this](const QString& text)
        {
            if (text.trimmed().isEmpty())
                loadCustomers();
        });

    refresh();
}

CustomersPage::~CustomersPage()
{
    delete ui;
}

void CustomersPage::refresh()
{
    loadCustomers();
    updateButtonStates();
}

void CustomersPage::updateButtonStates()
{
    bool hasTextName = !ui->lineEditAddCustomerName->text().trimmed().isEmpty();
    bool hasTextEmail = !ui->lineEditAddCustomerEmail->text().trimmed().isEmpty();
    bool hasTextPhone = !ui->lineEditAddCustomerPhone->text().trimmed().isEmpty();

    bool hasTextSearch = !ui->lineEditSearch->text().trimmed().isEmpty();


    bool hasSelection = ui->tableWidgetCustomers->currentRow() != -1;

    ui->pushButtonAddCustomer->setEnabled(hasTextName && hasTextEmail && hasTextPhone);
    ui->pushButtonDeleteCustomer->setEnabled(hasSelection);
    ui->pushButtonSearch->setEnabled(hasTextSearch);
}

void CustomersPage::loadCustomers() {
    ui->tableWidgetCustomers->setRowCount(0);

    vector<GetAllCustomers> customers = inventoryManager->getAllCustomersDB();

    for (const auto& customer : customers)
    {
        int row = ui->tableWidgetCustomers->rowCount();

        ui->tableWidgetCustomers->insertRow(row);

        ui->tableWidgetCustomers->setItem(row, 0, new QTableWidgetItem(
            QString::number(customer.customerID)));

        ui->tableWidgetCustomers->setItem(row, 1, new QTableWidgetItem(
            QString::fromStdString(customer.customerFullName)));

        ui->tableWidgetCustomers->setItem(row, 2, new QTableWidgetItem(
            QString::fromStdString(customer.customerEmail)));

        ui->tableWidgetCustomers->setItem(row, 3, new QTableWidgetItem(
            QString::fromStdString(customer.customerPhone)));
    }
}

void CustomersPage::on_pushButtonAddCustomer_clicked() {
    string customerName = ui->lineEditAddCustomerName->text().toStdString();
    string customerEmail = ui->lineEditAddCustomerEmail->text().toStdString();
    string customerPhone = ui->lineEditAddCustomerPhone->text().toStdString();

    if (customerName.empty() || customerEmail.empty() || customerPhone.empty()) {
        QMessageBox::warning(
            this,
            "Add Customer",
            "Name/Email/Phone can not be empty. Please try again."
        );
        return;
    }

    if (customerPhone.length() != 11)
    {
        QMessageBox::warning(
            this,
            "Add Customer",
            "Phone number must be exactly 11 digits."
        );
        return;
    }

    bool success = inventoryManager->addCustomerDB(customerName,customerPhone,customerEmail);

    if (!success) {
        QMessageBox::warning(
            this,
            "Add Customer",
            "Something went wrong, please try again."
        );
        return;
    }

    emit dataChanged();

    ui->lineEditAddCustomerName->clear();
    ui->lineEditAddCustomerEmail->clear();
    ui->lineEditAddCustomerPhone->clear();
}

void CustomersPage::on_pushButtonDeleteCustomer_clicked() {
    int row = ui->tableWidgetCustomers->currentRow();

    if (row == -1)
        return;

    int customerID = ui->tableWidgetCustomers->item(row, 0)
        ->text().toInt();

    bool success = inventoryManager->deleteCustomerDB(customerID);

    if (!success) {
        QMessageBox::warning(
            this,
            "Delete Customer",
            "This customer cannot be deleted because transaction history exists."
        );
        return;
    }

    emit dataChanged();
}

void CustomersPage::on_pushButtonSearch_clicked() {

    ui->tableWidgetCustomers->setRowCount(0);

    string keyword =
        ui->lineEditSearch->text().toStdString();

    vector<GetAllCustomers> customers = inventoryManager->searchCustomers(keyword);

    for (const auto& customer : customers)
    {
        int row = ui->tableWidgetCustomers->rowCount();

        ui->tableWidgetCustomers->insertRow(row);

        ui->tableWidgetCustomers->setItem(row, 0, new QTableWidgetItem(
            QString::number(customer.customerID)));

        ui->tableWidgetCustomers->setItem(row, 1, new QTableWidgetItem(
            QString::fromStdString(customer.customerFullName)));

        ui->tableWidgetCustomers->setItem(row, 2, new QTableWidgetItem(
            QString::fromStdString(customer.customerEmail)));

        ui->tableWidgetCustomers->setItem(row, 3, new QTableWidgetItem(
            QString::fromStdString(customer.customerPhone)));
    }
}