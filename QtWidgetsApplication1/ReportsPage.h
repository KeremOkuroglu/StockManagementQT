#pragma once

#include <QWidget>
#include "InventoryManager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class ReportsPageClass;
}
QT_END_NAMESPACE

class ReportsPage : public QWidget
{
    Q_OBJECT

public:
    explicit ReportsPage(
        InventoryManager* manager,
        QWidget* parent = nullptr);

    ~ReportsPage();

    void refresh();

private slots:

    void on_comboBoxReports_currentIndexChanged(int index);

private:

    void setupTable(const QStringList& headers);

    void loadReportTypes();

    void loadTotalProducts();

    void loadTotalCustomers();

    void loadTotalTransactions();

    void loadInventoryReport();

    void loadProductReport();

    void loadCustomerReport();

    void loadTransactionReport();

private:
    Ui::ReportsPageClass* ui;

    InventoryManager* inventoryManager;
};