#pragma once

#include <QWidget>
#include "InventoryManager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class UsersPageClass;
}
QT_END_NAMESPACE

class UsersPage : public QWidget
{
    Q_OBJECT

public:
    explicit UsersPage(
        InventoryManager* manager,
        QWidget* parent = nullptr);

    ~UsersPage();

    void refresh();

signals:
    void dataChanged();

private slots:
    void on_toolButtonPassword_clicked();

    void on_pushButtonAddUser_clicked();

    void on_pushButtonDeleteSelectedUser_clicked();

    void on_lineEditSearch_textChanged(const QString& text);

private:

    void loadUsers();

    void updateButtonStates();

private:
    Ui::UsersPageClass* ui;

    InventoryManager* inventoryManager;
};