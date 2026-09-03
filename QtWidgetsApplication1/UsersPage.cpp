#include "UsersPage.h"
#include "ui_UsersPage.h"
#include "qmessagebox.h"

UsersPage::UsersPage(
    InventoryManager* manager,
    QWidget* parent)
    : QWidget(parent),
    ui(new Ui::UsersPageClass),
    inventoryManager(manager)
{
    ui->setupUi(this);

    //Hide password
    ui->lineEditPassword->setEchoMode(QLineEdit::Password);

    //connects
    connect(ui->lineEditUsername, &QLineEdit::textChanged,
        this, &UsersPage::updateButtonStates);

    connect(ui->lineEditPassword, &QLineEdit::textChanged,
        this, &UsersPage::updateButtonStates);

    connect(ui->comboBoxRole, &QComboBox::currentIndexChanged,
        this, &UsersPage::updateButtonStates);

    connect(ui->tableWidgetUsers, &QTableWidget::itemSelectionChanged,
        this, &UsersPage::updateButtonStates);

    refresh();
}

UsersPage::~UsersPage()
{
    delete ui;
}

void UsersPage::refresh() {
    loadUsers();

    ui->lineEditUsername->clear();
    ui->lineEditPassword->clear();
    ui->comboBoxRole->setCurrentIndex(0);

    updateButtonStates();
}

void UsersPage::on_toolButtonPassword_clicked()
{
    if (ui->lineEditPassword->echoMode() == QLineEdit::Password)
    {
        ui->lineEditPassword->setEchoMode(QLineEdit::Normal);
        ui->toolButtonPassword->setText("Hide");
    }
    else
    {
        ui->lineEditPassword->setEchoMode(QLineEdit::Password);
        ui->toolButtonPassword->setText("Show");
    }
}

void UsersPage::updateButtonStates()
{
    bool hasTextName = !ui->lineEditUsername->text().trimmed().isEmpty();
    bool hasTextPassword = !ui->lineEditPassword->text().trimmed().isEmpty();
    bool roleSelected = ui->comboBoxRole->currentIndex() != 0;

    bool hasSelection = ui->tableWidgetUsers->currentRow() != -1;

    ui->comboBoxRole->setEnabled(hasTextName && hasTextPassword);
    ui->pushButtonAddUser->setEnabled(inventoryManager->isAdmin() && hasTextName
        && hasTextPassword && roleSelected);
    
    ui->pushButtonDeleteSelectedUser->setEnabled(inventoryManager->isAdmin() && hasSelection);
}

void UsersPage::loadUsers() {
    ui->tableWidgetUsers->setRowCount(0);

    vector<UserInfo> users = inventoryManager->getAllUsersDB();

    for (const auto& user : users)
    {
        int row = ui->tableWidgetUsers->rowCount();

        ui->tableWidgetUsers->insertRow(row);

        ui->tableWidgetUsers->setItem(row, 0, new QTableWidgetItem(
            QString::number(user.userID)));

        ui->tableWidgetUsers->setItem(row, 1, new QTableWidgetItem(
            QString::fromStdString(user.username)));

        string userRole = User::roleTypeToString(user.role);
        ui->tableWidgetUsers->setItem(row, 2, new QTableWidgetItem(
            QString::fromStdString(userRole)));

        ui->tableWidgetUsers->setItem(row, 3, new QTableWidgetItem(
            QString::fromStdString(user.createdAt)));
    }

    ui->tableWidgetUsers->setColumnHidden(0, true);
}


void UsersPage::on_pushButtonAddUser_clicked() {
    string username = ui->lineEditUsername->text().toStdString();
    string userPassword = ui->lineEditPassword->text().toStdString();
    RoleType userRole = User::stringToRoleType(ui->comboBoxRole->currentText().toStdString());

    if (username.empty() || userPassword.empty()) {
        QMessageBox::warning(
            this,
            "Add User",
            "Username and password cannot be empty."
        );
        return;
    }

    if (userPassword.length() < 8 || userPassword.length() > 12)
    {
        QMessageBox::warning(
            this,
            "Invalid Password",
            "Password must be between 8 and 12 characters.");
        return;
    }

    if (!inventoryManager->isAdmin() && userRole == RoleType::ADMIN)
    {
        QMessageBox::warning(
            this,
            "Permission Denied",
            "Only administrators can create administrator accounts."
        );
        return;
    }

    bool success = inventoryManager->addUserDB(username, userPassword, userRole);

    if (!success) {
        QMessageBox::warning(
            this,
            "Add User",
            "Something went wrong, please try again."
        );
        return;
    }

    emit dataChanged();

    ui->lineEditUsername->clear();
    ui->lineEditPassword->clear();
    ui->comboBoxRole->setCurrentIndex(0);
}

void UsersPage::on_pushButtonDeleteSelectedUser_clicked() {

    int row = ui->tableWidgetUsers->currentRow();

    if (row == -1)
        return;

    if (!inventoryManager->isAdmin()) {
        QMessageBox::warning(
            this,
            "Delete User",
            "You don't have permission for this."
        );
        return;
    }

    RoleType role = User::stringToRoleType(
        ui->tableWidgetUsers->item(row, 2)
        ->text()
        .toStdString()
    );

    if (role == RoleType::ADMIN) {
        QMessageBox::warning(
            this,
            "Delete User",
            "Administrator accounts cannot be deleted."
        );
        return;
    }


    int userID = ui->tableWidgetUsers->item(row, 0)
        ->text().toInt();

    if (userID == inventoryManager->getCurrentUser()->getUserID())
    {
        QMessageBox::warning(
            this,
            "Delete User",
            "You cannot delete your own account."
        );
        return;
    }

    bool success = inventoryManager->deleteUserDB(userID);

    if (!success) {
        QMessageBox::warning(
            this,
            "Delete User",
            "Something went wrong, please try again."
        );
        return;
    }

    emit dataChanged();
}

void UsersPage::on_lineEditSearch_textChanged(const QString& text) {
    ui->tableWidgetUsers->setRowCount(0);

    string keyword =
        ui->lineEditSearch->text().toStdString();

    vector<UserInfo> users = inventoryManager->searchUserDB(keyword);

    for (const auto& user : users)
    {
        int row = ui->tableWidgetUsers->rowCount();

        ui->tableWidgetUsers->insertRow(row);

        ui->tableWidgetUsers->setItem(row, 0, new QTableWidgetItem(
            QString::number(user.userID)));

        ui->tableWidgetUsers->setItem(row, 1, new QTableWidgetItem(
            QString::fromStdString(user.username)));

        string userRole = User::roleTypeToString(user.role);
        ui->tableWidgetUsers->setItem(row, 2, new QTableWidgetItem(
            QString::fromStdString(userRole)));

        ui->tableWidgetUsers->setItem(row, 3, new QTableWidgetItem(
            QString::fromStdString(user.createdAt)));
    }

    ui->tableWidgetUsers->setColumnHidden(0, true);
}