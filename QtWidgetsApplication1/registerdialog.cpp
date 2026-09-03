#include "registerdialog.h"
#include "ui_registerdialog.h"

RegisterDialog::RegisterDialog(InventoryManager *manager, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::RegisterDialog),
    inventoryManager(manager)
{
    ui->setupUi(this);

    ui->lineEditPassword->setEchoMode(QLineEdit::Password);
    ui->lineEditConfirmPassword->setEchoMode(QLineEdit::Password);

}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_toolButtonPassword_clicked()
{
    if (ui->lineEditPassword->echoMode() == QLineEdit::Password &&
        ui->lineEditConfirmPassword->echoMode() == QLineEdit::Password)
    {
        ui->lineEditPassword->setEchoMode(QLineEdit::Normal);
        ui->lineEditConfirmPassword->setEchoMode(QLineEdit::Normal);
        ui->toolButtonPassword->setText("Hide");
    }
    else
    {
        ui->lineEditPassword->setEchoMode(QLineEdit::Password);
        ui->lineEditConfirmPassword->setEchoMode(QLineEdit::Password);
        ui->toolButtonPassword->setText("Show");
    }
}


void RegisterDialog::on_buttonBox_accepted()
{
    string password1, password2;
    password1 = ui->lineEditPassword->text().toStdString();
    password2 = ui->lineEditConfirmPassword->text().toStdString();

    if (password1.length() < 8 || password1.length() > 12)
    {
        QMessageBox::warning(
            this,
            "Invalid Password",
            "Password must be between 8 and 12 characters.");
        return;
    }

    if (password1 != password2) {
        QMessageBox::warning(this, "Register", "Please Try Again.");
        return;
    }


    if(inventoryManager->registerUser(
            ui->lineEditUsername->text().toStdString(),
            ui->lineEditPassword->text().toStdString(),
            RoleType::EMPLOYEE)){
        accept();
    }
    else{
        QMessageBox::warning(this,"Register", "Please Try Again.");
    }
}


void RegisterDialog::on_buttonBox_rejected()
{
    reject();
}

