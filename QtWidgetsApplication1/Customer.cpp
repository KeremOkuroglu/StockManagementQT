#include "Customer.h"

Customer::Customer(int customerID, string customerFullName, string phone, string email) {
	this->customerID = customerID;
	this->customerFullName = customerFullName;
	this->phone = phone;
	this->email = email;

	createdTime = time(nullptr);
}

Customer::Customer() {
	customerID = 0;
	customerFullName = "";
	phone = "";
	email = "";
	createdTime = time(nullptr);
}

int Customer::getCustomerID() const {
	return customerID;
}

string Customer::getCustomerFullName() const {
	return customerFullName;
}