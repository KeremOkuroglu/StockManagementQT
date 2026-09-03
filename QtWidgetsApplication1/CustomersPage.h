#pragma once

#include <QWidget>
#include <vector>

class InventoryManager;

namespace Ui
{
    class CustomersPageClass;
}

class CustomersPage : public QWidget
{
    Q_OBJECT

public:
    explicit CustomersPage(
        InventoryManager* manager,
        QWidget* parent = nullptr);

    ~CustomersPage();

    void refresh();

signals:
    void dataChanged();

private slots:

    void on_pushButtonAddCustomer_clicked();

    void on_pushButtonDeleteCustomer_clicked();

    void on_pushButtonSearch_clicked();

private:

    void loadCustomers();

    void updateButtonStates();

private:

    Ui::CustomersPageClass* ui;

    InventoryManager* inventoryManager;
};