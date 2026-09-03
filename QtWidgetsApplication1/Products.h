#pragma once
#include "Category.h"

struct LowStockProduct
{
	string productName = "";
	int currentStock = 0;
	double price = 0.0;
};

//For StockManagementPage loadCustomers()
struct ProductInfo
{
	int productID = 0;
	int categoryID = 0;
	string productName = "";
	string categoryName = "";
	int currentStock = 0;
	double price = 0.0;
};

class Product {
private:
	int id;
	string name;
	int category_id;
	int currentStock;
	double price;

public:
	Product(int id, const string& name, int category_id, double price);

	int getProductID() const;
	string getName() const;
	int getCategoryID() const;
	int getCurrentStock() const;
	double getPrice() const;

};