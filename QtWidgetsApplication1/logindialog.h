#pragma once

#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <Qsettings.h>
#include <string>
#include "InventoryManager.h"

namespace Ui {
class LoginDialog;
}

class InventoryManager;

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(InventoryManager *manager,
                         QWidget *parent = nullptr);
    ~LoginDialog();

signals:
    void dataChanged();

private slots:
    void on_toolButtonPassword_clicked();

    void on_pushButtonLogin_clicked();

    void on_pushButtonRegister_clicked();

    void on_pushButtonCancel_clicked();

private:
    Ui::LoginDialog *ui;

    InventoryManager *inventoryManager;
};

#endif // LOGINDIALOG_H
