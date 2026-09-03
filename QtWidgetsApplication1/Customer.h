#pragma once
#include <iostream>
#include <string>
#include <ctime>
using namespace std;

struct GetAllCustomers
{
	int customerID = 0;
	string customerFullName = "";
	string customerPhone = "";
	string customerEmail = "";
	string createdAt = "";
};

class Customer {
private:
	int customerID;
	string customerFullName;
	string phone;
	string email;
	time_t createdTime;
	
public:
	Customer(int customerID, string customerFullName, string phone, string email);
	Customer();

	int getCustomerID() const;
	string getCustomerFullName() const;
};