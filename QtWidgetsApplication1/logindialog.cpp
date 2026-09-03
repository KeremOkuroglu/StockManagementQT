#include "logindialog.h"
#include "ui_logindialog.h"
#include "registerdialog.h"

LoginDialog::LoginDialog(InventoryManager *manager,
                         QWidget *parent)
    : QDialog(parent),
    ui(new Ui::LoginDialog),
    inventoryManager(manager)
{
    ui->setupUi(this);

    ui->lineEditPassword->setEchoMode(QLineEdit::Password);

    QSettings settings("StockManagement", "Inventory");

    bool remember = settings.value("remember", false).toBool();

    if (remember)
    {
        ui->checkBoxRememberMe->setChecked(true);

        ui->lineEditUserName->setText(
            settings.value("username").toString());
    }
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_toolButtonPassword_clicked()
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

void LoginDialog::on_pushButtonLogin_clicked()
{
    if(inventoryManager->login(
            ui->lineEditUserName->text().toStdString(),
            ui->lineEditPassword->text().toStdString())){
        
        QSettings settings("StockManagement", "Inventory");

        if (ui->checkBoxRememberMe->isChecked())
        {
            settings.setValue("remember", true);
            settings.setValue("username", ui->lineEditUserName->text());
        }
        else
        {
            settings.remove("remember");
            settings.remove("username");
        }
        emit dataChanged();

        accept();

    }
    else{
        QMessageBox::warning(this,"Login", "Username or password incorrect!");
    }
}


void LoginDialog::on_pushButtonRegister_clicked()
{
    RegisterDialog dialog(inventoryManager, this);

    dialog.exec();
}


void LoginDialog::on_pushButtonCancel_clicked()
{
    reject();
}