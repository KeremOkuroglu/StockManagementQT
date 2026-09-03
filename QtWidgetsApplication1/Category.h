#pragma once
#include <iostream>
#include <string>
using namespace std;

class Category {
private:
	int id;
	string name;

public:
	Category(int id, const string& name);

	int getID() const;
	string getName() const;
};