#include "Category.h"

Category::Category(int id, const string& name) {
	this->id = id;
	this->name = name;
}

int Category::getID() const {
	return id;
}

string Category::getName() const {
	return name;
}