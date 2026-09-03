#pragma once

#include <QWidget>
#include <vector>

class InventoryManager;

namespace Ui
{
    class ProductPageClass;
}

class ProductPage : public QWidget
{
    Q_OBJECT

public:
    explicit ProductPage(
        InventoryManager* manager,
        QWidget* parent = nullptr);

    ~ProductPage();

    void refresh();

signals:
    void dataChanged();

private slots:

    void on_pushButtonAddProduct_clicked();

    void on_pushButtonDeleteSelectedProduct_clicked();

private:

    void loadCategories();

    void loadProducts();

    void updateButtonStates();

private:

    Ui::ProductPageClass* ui;

    InventoryManager* inventoryManager;
};