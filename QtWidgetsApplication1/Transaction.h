#pragma once
#include "Products.h"
#include <ctime>

enum class TransactionType {
	PURCHASE,
	PRODUCTION,
	SALE,
	USAGE,
	LOSS,
	RETURN_REQUEST,
	RETURN_ACCEPTED,
	RETURN_REJECTED
};

struct RecentTransaction
{
	int transactionID = 0;
	int userID = 0;
	string userName = "";
	string productName = "";
	string customerName = "";
	TransactionType type;
	int quantity = 0;
	string timeStamp = "";
};

class Transaction {
private:
	int id;
	int product_id;
	TransactionType type;
	int quantity;
	time_t timeStamp;

	int userID;

public:
	Transaction(int id, int product_id, TransactionType type, int quantity, int userID);

	int getID() const;
	int getProductID() const;
	TransactionType getType() const;
	int getQuantity() const;
	time_t getTimeStamp() const;

	int getUserID() const;

	static string transactionToString(TransactionType type);
	static TransactionType stringToTransactionType(const string& type);
};