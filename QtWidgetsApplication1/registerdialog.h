#pragma once

#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include "InventoryManager.h"

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(InventoryManager *manager, QWidget *parent = nullptr);
    ~RegisterDialog();

private slots:
    void on_toolButtonPassword_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::RegisterDialog *ui;
    InventoryManager *inventoryManager;
};

#endif // REGISTERDIALOG_H
