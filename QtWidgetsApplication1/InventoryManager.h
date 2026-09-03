#pragma once
#include "Products.h"
#include "Transaction.h"
#include "Category.h"
#include "User.h"
#include "Customer.h"

#include "ReportModels.h"

#include "databaseManager.h"
#include <vector>
#include <cppconn/prepared_statement.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>

class InventoryManager {
private:
	User* currentUser;
	DatabaseManager databaseManager;

	int getPendingReturnQuantity(int productID, int customerID);

public:
	InventoryManager();

	bool addCategoryDB(const string& categoryName);
	bool deleteCategoryDB(int categoryID);

	bool addProductDB(const string& name, int categoryID, double price);
	bool deleteProductDB(int productID);

	int getReturnableQuantity(int productID, int customerID);

	//User login/logout/register etc. methods
	bool registerUser(const string& username, const string& password, RoleType role);
	bool login(const string& username, const string& password);
	void logout();


	//getter
	User* getCurrentUser();
	const User* getCurrentUser() const;
	//setter
	void setCurrentUser(User* user);

	//stock in out methods
	bool increaseStockDB(int productID, int amount);
	bool decreaseStockDB(int productID, int amount, int customerID = -1);

	//handle functions for processTransaction method
	bool handleStockIn(int productID, int amount);
	bool handleStockOut(int productID, int amount, int customerID = -1);
	bool handleRequest(int productID, int amount, int customerID = -1);

	//main function
	bool logTransaction(int productID, int quantity, TransactionType type, int customerID = -1);
	bool processTransactionDB(int productID, int quantity, TransactionType type, int customerID = -1);

	//list methods
	void listProductsDB();
	void listCategoriesDB();

	//methods for check the user
	bool isAdmin() const;
	bool isEmployee() const;

	//customer methods
	bool addCustomerDB(const string& customerName, const string& phone, const string& email);
	bool deleteCustomerDB(int customerID);
	vector<GetAllCustomers> searchCustomers(string customerName);

	void listCustomers();

	bool customerExists(int customerID);

	//QT methods
	//Dashboard
	int getProductCount();
	int getCategoryCount();
	int getCustomerCount();

	int getTotalStockValue();
	int getLowStockCount();
	int getTodayOrderCount();


	vector<RecentTransaction> getRecentTransactions(int limit = 5);
	vector<LowStockProduct> getLowStockProducts(int limit = 3);

	//Categories
	vector<Category> getAllCategoriesDB();

	//Products
	vector<Product> getAllProductDB();

	//Customers
	vector<GetAllCustomers> getAllCustomersDB();

	//Transactions
	vector<RecentTransaction> searchTransactions(string productName, string typeFilter = "");

	//Reports Page
	int getTransactionCount();
	vector<InventorySummaryReport> getAllInventorySummaryReport();
	vector<TransactionReport> getAllTransactionReport();
	vector<ProductReport> getAllProductReport();
	vector<CustomerReport> getAllCustomerReport();


	//Users Page
	vector<UserInfo> getAllUsersDB();
	vector<UserInfo> searchUserDB(string keyword);

	bool addUserDB(string userName, string password, RoleType role);

	bool deleteUserDB(int userID);

	bool hasTransaction(int userID);
	bool deactivateUser(int userID);
	bool permanentlyDeleteUser(int userID);

	//StockManagementPage
	vector<ProductInfo> getAllProductsInfoDB();
};