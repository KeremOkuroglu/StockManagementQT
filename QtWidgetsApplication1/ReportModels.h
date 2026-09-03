#pragma once

#include <string>

using std::string;

struct InventorySummaryReport 
{
    int productID = 0;
    string productName = "";
    string categoryName = "";

    int currentStock = 0;
    int availableStock = 0;

    double unitPrice = 0.0;
    double totalValue = 0.0;
};

struct TransactionReport
{
    int transactionID = 0;
    string productName = "";
    string customerName = "";
    string userName = "";
    string transactionType = "";
    int quantity = 0;
    string transactionDate = "";
};

struct ProductReport
{
    int productID = 0;
    string productName = "";
    int purchased = 0;
    int produced = 0;
    int sold = 0;
    int used = 0;
    int returned = 0;
    int lost = 0;
    int currentStock = 0;
};

struct CustomerReport
{
    int customerID = 0;
    string customerName = "";
    int totalPurchases = 0;
    int totalReturns = 0;
    string lastTransactionDate = "";
};