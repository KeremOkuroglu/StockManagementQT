#pragma once

#include <QWidget>

class InventoryManager;

namespace Ui
{
    class CategoriesClass;
}

class Categories : public QWidget
{
    Q_OBJECT

public:
    explicit Categories(InventoryManager* manager,
        QWidget* parent = nullptr);

    ~Categories();

    void refresh();

signals:
    void dataChanged();

private slots:

    void on_pushButtonAdd_clicked();

    void on_pushButtonDeleteSelected_clicked();

private:

    void loadCategories();

    void updateButtonStates();

private:

    Ui::CategoriesClass* ui;

    InventoryManager* inventoryManager;
};