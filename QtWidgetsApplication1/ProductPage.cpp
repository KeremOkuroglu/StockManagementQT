#include "ProductPage.h"
#include "ui_ProductPage.h"
#include "InventoryManager.h"

#include <QDoubleValidator>
#include <QRegularExpressionValidator>

#include "qmessagebox.h"

ProductPage::ProductPage(
    InventoryManager* manager,
    QWidget* parent)
    : QWidget(parent),
    ui(new Ui::ProductPageClass),
    inventoryManager(manager)
{
    ui->setupUi(this);

    //connect
    connect(ui->lineEditProductName, &QLineEdit::textChanged,
        this, &ProductPage::updateButtonStates);

    connect(ui->lineEditPrice, &QLineEdit::textChanged,
        this, &ProductPage::updateButtonStates);

    connect(ui->tableWidgetProduct, &QTableWidget::itemSelectionChanged,
        this, &ProductPage::updateButtonStates);

    //validator
    QDoubleValidator* validator = new QDoubleValidator(0.0, 1000000.0, 2, this);
    validator->setNotation(QDoubleValidator::StandardNotation);

    ui->lineEditPrice->setValidator(validator);

    refresh();
}

ProductPage::~ProductPage()
{
    delete ui;
}

void ProductPage::refresh()
{
    loadCategories();
    loadProducts();
    updateButtonStates();
}

void ProductPage::updateButtonStates()
{
    bool hasTextName = !ui->lineEditProductName->text().trimmed().isEmpty();
    bool hasTextPrice = !ui->lineEditPrice->text().trimmed().isEmpty();
    bool hasSelection = ui->tableWidgetProduct->currentRow() != -1;

    ui->pushButtonAddProduct->setEnabled(hasTextName && hasTextPrice);
    ui->pushButtonDeleteSelectedProduct->setEnabled(hasSelection);
}

void ProductPage::loadCategories()
{
    ui->comboBoxCategoryName->clear();

    vector<Category> categories = inventoryManager->getAllCategoriesDB();

    for (const Category& category : categories)
    {
        ui->comboBoxCategoryName->addItem(
            QString::fromStdString(category.getName()),
            category.getID()
        );
    }
}

void ProductPage::loadProducts()
{
    ui->tableWidgetProduct->setRowCount(0);

    vector<ProductInfo> products = inventoryManager->getAllProductsInfoDB();

    for (const auto& product : products)
    {
        int row = ui->tableWidgetProduct->rowCount();

        ui->tableWidgetProduct->insertRow(row);

        ui->tableWidgetProduct->setItem(row, 0, new QTableWidgetItem(
            QString::number(product.productID)));

        ui->tableWidgetProduct->setItem(row, 1, new QTableWidgetItem(
            QString::fromStdString(product.productName)));

        ui->tableWidgetProduct->setItem(row, 2, new QTableWidgetItem(
            QString::fromStdString(product.categoryName)));

        ui->tableWidgetProduct->setItem(row, 3, new QTableWidgetItem(
            QString::number(product.price)));
    }
}

void ProductPage::on_pushButtonAddProduct_clicked()
{
    string productName = ui->lineEditProductName->text().toStdString();
    int categoryID = ui->comboBoxCategoryName->currentData().toInt();
    double price = ui->lineEditPrice->text().toDouble();

    if (productName.empty()) {
        QMessageBox::warning(
            this,
            "Add Product",
            "Product Name cannot be empty."
        );
        return;
    }

    bool success = inventoryManager->addProductDB(productName, categoryID, price);

    if (!success) {
        QMessageBox::warning(
            this,
            "Add Product",
            "Something went wrong, please try again."
        );
        return;
    }

    emit dataChanged();

    ui->lineEditPrice->clear();
    ui->lineEditProductName->clear();
}

void ProductPage::on_pushButtonDeleteSelectedProduct_clicked()
{
    int row = ui->tableWidgetProduct->currentRow();

    if (row == -1)
        return;

    int productID = ui->tableWidgetProduct->item(row, 0)
        ->text().toInt();

    bool success = inventoryManager->deleteProductDB(productID);

    if (!success) {
        QMessageBox::warning(
            this,
            "Delete Product",
            "The product cannot be deleted because its current stock is not zero. "
            "Remove or reassign those products first."
        );
        return;
    }

    emit dataChanged();
}