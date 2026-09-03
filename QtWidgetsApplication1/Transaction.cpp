#include "Transaction.h"

Transaction::Transaction(int id, int product_id, TransactionType type, int quantity, int userID) {
	this->id = id;
	this->product_id = product_id;
	this->type = type;
	this->quantity = quantity;
	this->userID = userID;

	timeStamp = time(nullptr);
}

string Transaction::transactionToString(TransactionType type) {
	switch (type) {
	case TransactionType::PURCHASE: return "PURCHASE";
	case TransactionType::PRODUCTION: return "PRODUCTION";
	case TransactionType::SALE: return "SALE";
	case TransactionType::USAGE: return "USAGE";
	case TransactionType::LOSS: return "LOSS";
	case TransactionType::RETURN_REQUEST: return "RETURN REQUEST";
	case TransactionType::RETURN_ACCEPTED: return "RETURN ACCEPTED";
	case TransactionType::RETURN_REJECTED: return "RETURN REJECTED";
	default: return "UNKNOWN";
	}
}

TransactionType Transaction::stringToTransactionType(const string& type)
{
	if (type == "PURCHASE") return TransactionType::PURCHASE;
	if (type == "PRODUCTION") return TransactionType::PRODUCTION;
	if (type == "SALE") return TransactionType::SALE;
	if (type == "USAGE") return TransactionType::USAGE;
	if (type == "LOSS") return TransactionType::LOSS;
	if (type == "RETURN REQUEST") return TransactionType::RETURN_REQUEST;
	if (type == "RETURN ACCEPTED") return TransactionType::RETURN_ACCEPTED;
	if (type == "RETURN REJECTED") return TransactionType::RETURN_REJECTED;

	throw runtime_error("Unknown transaction type.");
}

int Transaction::getID() const {
	return id;
}

int Transaction::getProductID() const {
	return product_id;
}

TransactionType Transaction::getType() const {
	return type;
}

int Transaction::getQuantity() const {
	return quantity;
}

time_t Transaction::getTimeStamp() const {
	return timeStamp;
}

int Transaction::getUserID() const{
	return userID;
}