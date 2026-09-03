#include "Products.h"

Product::Product(int id, const string& name, int category_id, double price) {
	this->id = id;
	this->name = name;
	this->category_id = category_id;
	this->price = price;
	
	//default values for initialization
	currentStock = 0;
}

//Getters
int Product::getProductID() const {
	return id;
}

string Product::getName() const {
	return name;
}

int Product::getCategoryID() const {
	return category_id;
}

int Product::getCurrentStock() const {
	return currentStock;
}

double Product::getPrice() const {
	return price;
}