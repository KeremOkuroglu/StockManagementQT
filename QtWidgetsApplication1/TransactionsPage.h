#pragma once

#include <QWidget>
#include <vector>

class InventoryManager;

namespace Ui
{
    class TransactionsPageClass;
}

class TransactionsPage : public QWidget
{
    Q_OBJECT

public:
    explicit TransactionsPage(
        InventoryManager* manager,
        QWidget* parent = nullptr);

    ~TransactionsPage();

    void refresh();

signals:
    void dataChanged();

private slots:

    void on_pushButtonSearch_clicked();

private:

    void loadTransactions();

    void loadTransactionTypes();

    void updateButtonStates();

private:

    Ui::TransactionsPageClass* ui;

    InventoryManager* inventoryManager;
};