#include "Categories.h"
#include "ui_Categories.h"
#include "InventoryManager.h"

#include <QRegularExpressionValidator>

#include "qmessagebox.h"

Categories::Categories(
    InventoryManager* manager,
    QWidget* parent)
    : QWidget(parent),
    ui(new Ui::CategoriesClass),
    inventoryManager(manager)
{
    ui->setupUi(this);

    connect(ui->lineEditAdd, &QLineEdit::textChanged,
        this, &Categories::updateButtonStates);

    connect(ui->tableWidgetExistingCategories, &QTableWidget::itemSelectionChanged,
        this, &Categories::updateButtonStates);

    refresh();
}

Categories::~Categories()
{
    delete ui;
}

void Categories::refresh()
{
    loadCategories();
    updateButtonStates();
}

void Categories::updateButtonStates()
{
    bool hasText = !ui->lineEditAdd->text().trimmed().isEmpty();
    bool hasSelection = ui->tableWidgetExistingCategories->currentRow() != -1;

    ui->pushButtonAdd->setEnabled(hasText);
    ui->pushButtonDeleteSelected->setEnabled(hasSelection);
}

void Categories::loadCategories()
{
    ui->tableWidgetExistingCategories->setRowCount(0);

    vector<Category> categories = inventoryManager->getAllCategoriesDB();

    for (const auto& category : categories)
    {
        int row = ui->tableWidgetExistingCategories->rowCount();

        ui->tableWidgetExistingCategories->insertRow(row);

        ui->tableWidgetExistingCategories->setItem(row, 0, new QTableWidgetItem(
            QString::number(category.getID())));

        ui->tableWidgetExistingCategories->setItem(row, 1, new QTableWidgetItem(
            QString::fromStdString(category.getName())));
    }
}

void Categories::on_pushButtonAdd_clicked()
{
    string categoryName = ui->lineEditAdd->text().toStdString();

    if (categoryName.empty()) {
        QMessageBox::warning(
            this,
            "Add Category",
            "Category Name cannot be empty."
        );
        return;
    }

    bool success = inventoryManager->addCategoryDB(categoryName);

    if (!success) {
        QMessageBox::warning(
            this,
            "Add Category",
            "There is already a category with that name. Please try again."
        );
        return;
    }

    emit dataChanged();

    ui->lineEditAdd->clear();
}

void Categories::on_pushButtonDeleteSelected_clicked()
{
    int row = ui->tableWidgetExistingCategories->currentRow();

    if (row == -1)
        return;

    int categoryID = ui->tableWidgetExistingCategories->item(row, 0)
        ->text().toInt();

    bool success = inventoryManager->deleteCategoryDB(categoryID);
    if (!success) {
        QMessageBox::warning(
            this,
            "Delete Category",
            "This category cannot be deleted because "
            "it is currently assigned to one or more products. "
            "Remove or reassign those products first."
        );
        return;
    }
    emit dataChanged();
}