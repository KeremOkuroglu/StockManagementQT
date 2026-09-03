#include "InventoryManager.h"
#include "qmessagebox.h"

InventoryManager::InventoryManager() {
	currentUser = nullptr;
	databaseManager.connect();
}

//Database'e ekleme fonksiyonu, addCategoryDB
bool InventoryManager::addCategoryDB(const string& categoryName) {
	try {
		sql::Connection* con = databaseManager.getConnection();

		sql::PreparedStatement* pstmt = con->prepareStatement(
			"INSERT INTO category (categoryName) "
			"VALUES (?)"
		);

		pstmt->setString(1, categoryName);

		pstmt->execute();

		delete pstmt;
		return true;
	}
	catch (sql::SQLException& e) {
		cout << e.what() << endl;
		return false;
	}
}

bool InventoryManager::deleteCategoryDB(int categoryID) {
	try {
		sql::Connection* con = databaseManager.getConnection();

		sql::PreparedStatement* pstmt = con->prepareStatement(
			"DELETE FROM category "
			"WHERE categoryID = ?"
		);

		pstmt->setInt(1, categoryID);

		int affectedRows= pstmt->executeUpdate();

		delete pstmt;
		return affectedRows > 0;
	}
	catch (sql::SQLException& e) {
		cout << e.what() << endl;
		return false;
	}
}
//Category kismi bitisi


//addProduct for database 
bool InventoryManager::addProductDB(const string& name, int categoryID, double price) {
		try
		{
			// bindary condition for Negative values
			if (price < 0 || categoryID < 0) {
				cout << "Invalid values!" << endl;
				return false;
			}

			sql::Connection* con = databaseManager.getConnection();

			//Product name exists?
			sql::PreparedStatement* checkProduct = con->prepareStatement(
					"SELECT COUNT(*) "
					"FROM product "
					"WHERE productName = ?"
				);

			checkProduct->setString(1, name);

			sql::ResultSet* productResult =
				checkProduct->executeQuery();

			productResult->next();

			if (productResult->getInt(1) > 0) {
				cout << "Product already exists!"
					<< endl;

				delete productResult;
				delete checkProduct;

				return false;
			}

			delete productResult;
			delete checkProduct;

			//Is Category exists?
			sql::PreparedStatement* checkCategory = con->prepareStatement(
					"SELECT COUNT(*) "
					"FROM category "
					"WHERE categoryID = ?"
				);

			checkCategory->setInt(1, categoryID);

			sql::ResultSet* categoryResult = checkCategory->executeQuery();

			categoryResult->next();

			if (categoryResult->getInt(1) == 0) {
				cout << "Invalid category!" << endl;

				delete categoryResult;
				delete checkCategory;

				return false;
			}

			delete categoryResult;
			delete checkCategory;

			//Insert product
			sql::PreparedStatement* insertStmt = con->prepareStatement(
					"INSERT INTO product "
					"(productName, categoryID, price) "
					"VALUES (?, ?, ?)"
				);

			insertStmt->setString(1, name);

			insertStmt->setInt(2, categoryID);

			insertStmt->setDouble(3, price);

			insertStmt->execute();

			delete insertStmt;

			return true;
		}
		catch (sql::SQLException& e){
			cout << "SQL Error: "
				<< e.what()
				<< endl;

			return false;
		}
}

//delete product
bool InventoryManager::deleteProductDB(int productID) {
	try {
		sql::Connection* con = databaseManager.getConnection();

		sql::PreparedStatement* pstmt = con->prepareStatement(
			"DELETE FROM product "
			"WHERE productID = ?"
		);

		pstmt->setInt(1, productID);

		int affectedRows = pstmt->executeUpdate();

		delete pstmt;
		return affectedRows > 0;
	}
	catch (sql::SQLException& e) {
		cout << e.what() << endl;
		return false;
	}
}
//end of addProductDB 


///getter/setter
User* InventoryManager::getCurrentUser() {
	return currentUser;
}

const User* InventoryManager::getCurrentUser() const {
	return currentUser;
}

//setter
void InventoryManager::setCurrentUser(User* user) {
	this->currentUser = user;
}


//stock in out methods
//stock in method
bool InventoryManager::increaseStockDB(int productID, int amount) {
	try {
		if (amount <= 0) {
			cout << "quantity cannot be negative or zero" << endl;
			return false;
		}

		sql::Connection* con = databaseManager.getConnection();

		sql::PreparedStatement* updateStmt = con->prepareStatement(
			"UPDATE product "
			"SET currentStock = currentStock + ? "
			"WHERE productID = ?"
		);

		updateStmt->setInt(1, amount);
		updateStmt->setInt(2, productID);
		int affectedRows = updateStmt->executeUpdate();

		delete updateStmt;

		return affectedRows > 0;
	}
	catch (sql::SQLException& e) {
		cout << "SQL ERROR: " << e.what() << endl;
		return false;
	}
}

//stock out method
bool InventoryManager::decreaseStockDB(int productID, int amount, int customerID) {
	try {
		if (amount < 0) {
			cout << "quantity cannot be negative" << endl;
			return false;
		}

		sql::Connection* con = databaseManager.getConnection();

		//Current stock
		sql::PreparedStatement* checkStmt =
			con->prepareStatement(
				"SELECT currentStock "
				"FROM Product "
				"WHERE productID = ?"
			);
		checkStmt->setInt(1, productID);

		sql::ResultSet* res = checkStmt->executeQuery();

		// Product not found
		if (!res->next()) {
			cout << "Product not found!" << endl;

			delete res;
			delete checkStmt;

			return false;
		}

		int currentStock = res->getInt("currentStock");

		delete res;
		delete checkStmt;

		// Is there enoguh stock?
		if (currentStock < amount) {
			cout << "Not enough stock!" << endl;
			return false;
		}

		//update currentStock
		sql::PreparedStatement* updateStmt = con->prepareStatement(
			"UPDATE product "
			"SET currentStock = currentStock - ? "
			"WHERE productID = ?"
		);

		updateStmt->setInt(1, amount);
		updateStmt->setInt(2, productID);

		int affectedRows = updateStmt->executeUpdate();
		delete updateStmt;

		return affectedRows > 0;
	}
	catch (sql::SQLException& e) {
		cout << "SQL ERROR: " << e.what() << endl;
		return false;
	}
}


//handle methods
bool InventoryManager::handleStockIn(int productID, int amount) {
	return increaseStockDB(productID, amount);
}

bool InventoryManager::handleStockOut(int productID, int amount, int customerID) {
	return decreaseStockDB(productID, amount, customerID);
}

bool InventoryManager::handleRequest(int productID, int amount, int customerID) {
	if (amount > getReturnableQuantity(productID, customerID)) {
		cout << "Amount cannot be higher than returnable quantity." << endl;
		return false;
	}
	if (!customerExists(customerID)) {
		cout << "Customer doesn't exist!\n";
		return false;
	}

	return true;
}




////Main method
bool InventoryManager::processTransactionDB(int productID, int quantity, TransactionType type, int customerID) {

	bool success = false;

	if (currentUser == nullptr)
	{
		cout << "Please login first!" << endl;
		return false;
	}

	switch (type) {
	case TransactionType::PURCHASE:
	case TransactionType::PRODUCTION:
	{
		success = handleStockIn(productID, quantity);
		break;
	}

	case TransactionType::SALE:
	{
		success = handleStockOut(productID, quantity, customerID);
		break;
	}
	case TransactionType::USAGE:
	{
		success = handleStockOut(productID, quantity);
		break;
	}

	case TransactionType::LOSS:
	{
		if (!isAdmin()) {
			cout << "Only admin can perform this transaction.\n";
			return false;
		}
		else
			success = handleStockOut(productID, quantity);
		break;
	}
	case TransactionType::RETURN_REQUEST:
	{
		success = handleRequest(productID, quantity, customerID);
		break;
	}
	case TransactionType::RETURN_ACCEPTED:
	{
		if (!isAdmin())
			return false;
		else {
			int pending = getPendingReturnQuantity(productID, customerID);

			if (quantity > pending) {
				cout << "Return quantity exceeds pending requests.\n";
				return false;
			}

			success = handleStockIn(productID, quantity);
		}
		break;
	}

	case TransactionType::RETURN_REJECTED:
	{
		if (!isAdmin())
			return false;
		else {
			int pending = getPendingReturnQuantity(productID, customerID);

			if (pending < quantity) {
				cout << "Return quantity exceeds pending requests.\n";
				return false;
			}
			success = true;
		}
		break;
	}

	default:
	{
		cout << "gecersiz islem!" << endl; //debug icin
		return success;
	}
	}

	if (success)
	{

		return logTransaction(productID, quantity, type, customerID);
	}

	return false;
	/*if (success)
		return logTransaction(productID, quantity, type, customerID);

	return success;*/
}


bool InventoryManager::logTransaction(int productID, int quantity, TransactionType type, int customerID) {
	try {
		if (quantity < 0) {
			cout << "quantity cannot be negative" << endl;
			return false;
		}


		sql::Connection* con = databaseManager.getConnection();

		sql::PreparedStatement* stmt = con->prepareStatement(
			"INSERT into stockTransaction "
			"(transactiontype, quantity, productID, userID, customerID) "
			"VALUES (?, ?, ?, ?, ?)"
		);

		
		stmt->setString(1, Transaction::transactionToString(type));
		stmt->setInt(2, quantity);
		stmt->setInt(3, productID);
		stmt->setInt(4, currentUser->getUserID());
		if (customerID == -1)
		{
			stmt->setNull(5, sql::DataType::INTEGER);
		}
		else
		{
			stmt->setInt(5, customerID);
		}

		stmt->execute();

		delete stmt;

		return true;
	}
	catch (sql::SQLException& e) {
		cout << "SQL ERROR: " << e.what() << endl;
		return false;
	}
}

//User check methods
bool InventoryManager::isAdmin() const
{
	if (currentUser && currentUser->getRoleType() == RoleType::ADMIN)
		return true;
	else
		return false;
}

bool InventoryManager::isEmployee() const
{
	return currentUser && currentUser->getRoleType() == RoleType::EMPLOYEE;
}


//User methods (login/logout/register)
bool InventoryManager::registerUser(const string& username, const string& password, RoleType role) {
	try {
		sql::Connection* con = databaseManager.getConnection();

		sql::PreparedStatement* stmt = con->prepareStatement(
			"INSERT into users "
			"(userName, userRole, userPassword) "
			"VALUES (?, ?, ?)"
		);

		stmt->setString(1, username);
		stmt->setString(2, User::roleTypeToString(role));
		stmt->setString(3, password);

		stmt->execute();

		delete stmt;
		return true;
	}
	catch (sql::SQLException& e) {
		cout << "SQL ERROR: " << e.what() << endl;
		return false;
	}
}

bool InventoryManager::login(const string& username, const string& password) {
	try {
		sql::Connection* con = databaseManager.getConnection();

		//Current stock
		sql::PreparedStatement* stmt =
			con->prepareStatement(
				"SELECT * "
				"FROM Users "
				"WHERE userName = ? "
				"AND isActive = TRUE"
			);
		
		stmt->setString(1, username);

		sql::ResultSet* res = stmt->executeQuery();

		// User not found
		if (!res->next()) {
			cout << "User not found!" << endl;

			delete res;
			delete stmt;

			return false;
		}

		string sqlPassword = res->getString("userPassword");

		if (sqlPassword != password) {
			cout << "Wrong password!" << endl;

			delete res;
			delete stmt;

			return false;
		}

		if (currentUser != nullptr) {
			delete currentUser;
			currentUser = nullptr;
		}

		//login
		currentUser = new User(
			res->getInt("userID"),
			res->getString("userName"),
			sqlPassword,
			User::stringToRoleType(res->getString("userRole"))
		);

		delete res;
		delete stmt;

		return true;
	}
	catch (sql::SQLException& e) {
		cout << "SQL ERROR: " << e.what() << endl;
		return false;
	}
}

void InventoryManager::logout() {
	if (currentUser != nullptr) {
		delete currentUser;
		currentUser = nullptr;
	}
}

int InventoryManager::getPendingReturnQuantity(int productID, int customerID) {
	try {
		sql::Connection* con = databaseManager.getConnection();

		//Current stock
		sql::PreparedStatement* stmt =
			con->prepareStatement(
				"SELECT "
				"COALESCE(SUM( "
				"CASE "
				"WHEN transactionType = 'RETURN REQUEST' THEN quantity "
				"WHEN transactionType = 'RETURN ACCEPTED' THEN -quantity "
				"WHEN transactionType = 'RETURN REJECTED' THEN -quantity "
				"ELSE 0 "
				"END "
				"),0) AS pending "
				"FROM stockTransaction "
				"WHERE productID = ? AND customerID = ?"
			);

		stmt->setInt(1, productID);
		stmt->setInt(2, customerID);

		sql::ResultSet* res = stmt->executeQuery();
		res->next();

		int pending = res->getInt("pending");

		delete res;
		delete stmt;

		return pending;
	}
	catch (sql::SQLException& e) {
		throw std::runtime_error(
			"Failed to retrieve pending return quantity: " +
			string(e.what()));
	}
}

int InventoryManager::getReturnableQuantity(int productID, int customerID) {
	try {
		sql::Connection* con = databaseManager.getConnection();

		//Current stock
		sql::PreparedStatement* stmt =
			con->prepareStatement(
				"SELECT "
				"COALESCE(SUM( "
				"CASE "
				"WHEN transactionType = 'SALE' THEN quantity "
				"WHEN transactionType = 'RETURN REQUEST' THEN -quantity "
				"WHEN transactionType = 'RETURN ACCEPTED' THEN -quantity "
				"WHEN transactionType = 'RETURN REJECTED' THEN quantity "
				"ELSE 0 "
				"END "
				"),0) AS returnable "
				"FROM stockTransaction "
				"WHERE customerID = ? AND productID = ?"
			);

		stmt->setInt(1, customerID);
		stmt->setInt(2, productID);

		sql::ResultSet* res = stmt->executeQuery();
		res->next();

		int returnable = res->getInt("returnable");

		delete res;
		delete stmt;

		return returnable;
	}
	catch (sql::SQLException& e) {
		throw runtime_error(
			"Failed to retrieve SALE information: " +
			string(e.what()));
	}
}

//Customer methods
bool InventoryManager::addCustomerDB(const string& customerName, const string& phone, const string& email) {
	try {
		sql::Connection* con = databaseManager.getConnection();

		sql::PreparedStatement* stmt = con->prepareStatement(
			"INSERT into customers "
			"(customerFullName, Phone, email) "
			"VALUES (?, ?, ?)"
		);

		stmt->setString(1, customerName);
		stmt->setString(2, phone);
		stmt->setString(3, email);

		stmt->execute();

		delete stmt;
		return true;
	}
	catch (sql::SQLException& e) {
		cout << "SQL ERROR: " << e.what() << endl;
		return false;
	}
}

bool InventoryManager::deleteCustomerDB(int customerID) {
	try {
		sql::Connection* con = databaseManager.getConnection();

		sql::PreparedStatement* pstmt = con->prepareStatement(
			"DELETE FROM customers "
			"WHERE customerID = ?"
		);

		pstmt->setInt(1, customerID);

		int affectedRows = pstmt->executeUpdate();

		delete pstmt;
		return affectedRows > 0;
	}
	catch (sql::SQLException& e) {
		cout << e.what() << endl;
		return false;
	}
}

vector<GetAllCustomers> InventoryManager::searchCustomers(string customerName) {
	sql::Connection* con = databaseManager.getConnection();

	sql::PreparedStatement* stmt =
		con->prepareStatement(
			"SELECT * "
			"FROM customers "
			"WHERE customerFullName LIKE ? "
			"OR email LIKE ?"
		);

	string keyword = "%" + customerName + "%";

	stmt->setString(1, keyword);
	stmt->setString(2, keyword);

	sql::ResultSet* res = stmt->executeQuery();

	vector<GetAllCustomers> customers;

	while (res->next())
	{
		GetAllCustomers customer;

		customer.customerID = res->getInt("customerID");
		customer.customerFullName = res->getString("customerFullName");
		customer.customerPhone = res->getString("Phone");
		customer.customerEmail = res->getString("email");
		customer.createdAt = res->getString("createdAt");

		customers.push_back(customer);
	}

	delete res;
	delete stmt;

	return customers;
}


void InventoryManager::listCustomers() {
	try {
		sql::Statement* stmt = databaseManager.getConnection()->createStatement();

		sql::ResultSet* res = stmt->executeQuery(
			"SELECT * FROM customers"
		);

		cout << "\n----------- Customers -------------\n";

		while (res->next())
		{
			cout
				<< "\nCustomer ID: "
				<< res->getInt("customerID")

				<< "\nCustomer Full Name: "
				<< res->getString("customerFullName")

				<< "\nPhone: "
				<< res->getString("Phone")

				<< "\nEmail: "
				<< res->getString("email")

				<< "\n------------------------"
				<< endl;
		}
		delete res;
		delete stmt;
	}
	catch (sql::SQLException& e) {
		cout << "SQL Error: " << e.what() << endl;
	}
}

//customer exist metodunu ekle
bool InventoryManager::customerExists(int customerID) {
	try {
		sql::Connection* con = databaseManager.getConnection();

		sql::PreparedStatement* stmt =
			con->prepareStatement(
				"SELECT COUNT(*) "
				"FROM Customers "
				"WHERE customerID = ?"
			);

		stmt->setInt(1, customerID);

		sql::ResultSet* res = stmt->executeQuery();

		res->next();

		bool exists = res->getInt(1) > 0;

		delete res;
		delete stmt;

		return exists;
	}
	catch (sql::SQLException& e) {
		cout << "SQL Error: " << e.what() << endl;
		return false;
	}
}



//--------------------
//QT Dashboard Methods
int InventoryManager::getProductCount() {
	try {
		sql::Connection* con = databaseManager.getConnection();

		sql::PreparedStatement* stmt =
			con->prepareStatement(
				"SELECT COUNT(*) "
				"FROM product"
			);
		sql::ResultSet* res = stmt->executeQuery();

		int productCount = 0;

		if (res->next())
			productCount = res->getInt(1);

		delete res;
		delete stmt;

		return productCount;
	}
	catch (sql::SQLException& e) {
		cout << "SQL Error: " << e.what() << endl;
		return 0;
	}
}



int InventoryManager::getCategoryCount() {
	try {
		sql::Connection* con = databaseManager.getConnection();

		sql::PreparedStatement* stmt =
			con->prepareStatement(
				"SELECT COUNT(*) "
				"FROM category"
			);
		sql::ResultSet* res = stmt->executeQuery();

		int categoryCount = 0;

		if (res->next())
			categoryCount = res->getInt(1);

		delete res;
		delete stmt;

		return categoryCount;
	}
	catch (sql::SQLException& e) {
		cout << "SQL Error: " << e.what() << endl;
		return 0;
	}
}



int InventoryManager::getCustomerCount() {
	try {
		sql::Connection* con = databaseManager.getConnection();

		sql::PreparedStatement* stmt =
			con->prepareStatement(
				"SELECT COUNT(*) "
				"FROM customers"
			);
		sql::ResultSet* res = stmt->executeQuery();

		int userCount = 0;

		if (res->next())
			userCount = res->getInt(1);

		delete res;
		delete stmt;

		return userCount;
	}
	catch (sql::SQLException& e) {
		cout << "SQL Error: " << e.what() << endl;
		return 0;
	}
}



int InventoryManager::getTotalStockValue() {
	try {
		sql::Connection* con = databaseManager.getConnection();

		sql::PreparedStatement* stmt =
			con->prepareStatement(
				"SELECT SUM(currentStock * price) "
				"FROM product"
			);
		sql::ResultSet* res = stmt->executeQuery();

		int totalStockValue = 0;

		if (res->next())
			totalStockValue = res->getInt(1);

		delete res;
		delete stmt;

		return totalStockValue;
	}
	catch (sql::SQLException& e) {
		cout << "SQL Error: " << e.what() << endl;
		return 0;
	}
}



int InventoryManager::getLowStockCount() {
	try {
		sql::Connection* con = databaseManager.getConnection();

		sql::PreparedStatement* stmt =
			con->prepareStatement(
				"SELECT COUNT(*) "
				"FROM product "
				"WHERE currentStock < 10"
			);
		sql::ResultSet* res = stmt->executeQuery();

		int lowStockValue = 0;

		if (res->next())
			lowStockValue = res->getInt(1);

		delete res;
		delete stmt;

		return lowStockValue;
	}
	catch (sql::SQLException& e) {
		cout << "SQL Error: " << e.what() << endl;
		return 0;
	}
}



int InventoryManager::getTodayOrderCount() {
	try {
		sql::Connection* con = databaseManager.getConnection();

		sql::PreparedStatement* stmt =
			con->prepareStatement(
				"SELECT COUNT(*) "
				"FROM stockTransaction "
				"WHERE DATE(timeStamp) = CURDATE()"
			);
		sql::ResultSet* res = stmt->executeQuery();

		int todayOrderCount = 0;

		if (res->next())
			todayOrderCount = res->getInt(1);

		delete res;
		delete stmt;

		return todayOrderCount;
	}
	catch (sql::SQLException& e) {
		cout << "SQL Error: " << e.what() << endl;
		return 0;
	}
}



vector<RecentTransaction> InventoryManager::getRecentTransactions(int limit) {

		sql::Connection* con = databaseManager.getConnection();

		string query = "SELECT st.transactionID, u.userID ,st.timeStamp, p.productName, " 
				"st.transactionType, st.quantity, "
				"c.customerFullName, u.userName "
				"FROM stockTransaction st "
				"INNER JOIN Product p ON st.productID = p.productID "
				"LEFT JOIN Customers c ON st.customerID = c.customerID "
				"JOIN Users u ON st.userID = u.userID "
				"ORDER BY st.timeStamp DESC";
		if (limit > 0)
			query += " LIMIT ?";

		sql::PreparedStatement* stmt =
			con->prepareStatement(query);

		if (limit > 0)
			stmt->setInt(1, limit);

		sql::ResultSet* res = stmt->executeQuery();

		vector<RecentTransaction> transactions;

		while (res->next())
		{
			RecentTransaction transaction;

			transaction.transactionID = res->getInt("transactionID");
			transaction.userID = res->getInt("userID");

			transaction.productName = res->getString("productName");

			transaction.type =
				Transaction::stringToTransactionType(
					res->getString("transactionType"));

			transaction.quantity = res->getInt("quantity");
			transaction.timeStamp = res->getString("timeStamp");

			transaction.customerName = res->getString("customerFullName");
			transaction.productName = res->getString("productName");
			transaction.userName = res->getString("userName");

			transactions.push_back(transaction);
		}

		delete res;
		delete stmt;

		return transactions;
}



vector<LowStockProduct> InventoryManager::getLowStockProducts(int limit) {
	sql::Connection* con = databaseManager.getConnection();

	string query = "SELECT productName, currentStock, price "
		"FROM product "
		"WHERE currentStock < 10";

	if (limit > 0)
		query += " LIMIT ?";

	sql::PreparedStatement* stmt =
		con->prepareStatement(query);

	if (limit > 0)
		stmt->setInt(1, limit);

	sql::ResultSet* res = stmt->executeQuery();

	vector<LowStockProduct> products;

	while (res->next())
	{
		LowStockProduct product;

		product.productName = res->getString("productName");
		product.currentStock = res->getInt("currentStock");
		product.price = res->getInt("price");

		products.push_back(product);
	}

	delete res;
	delete stmt;

	return products;
}


//Category Page
vector<Category> InventoryManager::getAllCategoriesDB() {
	sql::Connection* con = databaseManager.getConnection();

	sql::PreparedStatement* stmt =
		con->prepareStatement(
			"SELECT * "
			"FROM category"
		);
	sql::ResultSet* res = stmt->executeQuery();

	vector<Category> categories;

	while (res->next())
	{
		int id = res->getInt("categoryID");
		string name = res->getString("categoryName");

		Category category(id, name);
		categories.push_back(category);
	}

	delete res;
	delete stmt;

	return categories;
}

//Product Page
vector<Product> InventoryManager::getAllProductDB() {
	sql::Connection* con = databaseManager.getConnection();

	sql::PreparedStatement* stmt =
		con->prepareStatement(
			"SELECT * "
			"FROM product"
		);
	sql::ResultSet* res = stmt->executeQuery();

	vector<Product> products;

	while (res->next())
	{
		int id = res->getInt("productID");
		string name = res->getString("productName");
		int categoryID = res->getInt("categoryID");
		double price = res->getDouble("price");

		Product product(id, name, categoryID, price);
		products.push_back(product);
	}

	delete res;
	delete stmt;

	return products;
}


//Customer Page
vector<GetAllCustomers> InventoryManager::getAllCustomersDB() {
	sql::Connection* con = databaseManager.getConnection();

	sql::PreparedStatement* stmt =
		con->prepareStatement(
			"SELECT * "
			"FROM customers"
		);
	sql::ResultSet* res = stmt->executeQuery();

	vector<GetAllCustomers> customers;

	while (res->next())
	{
		GetAllCustomers customer;

		customer.customerID = res->getInt("customerID");
		customer.customerFullName = res->getString("customerFullName");
		customer.customerPhone = res->getString("Phone");
		customer.customerEmail = res->getString("email");
		customer.createdAt = res->getString("createdAt");

		customers.push_back(customer);
	}

	delete res;
	delete stmt;

	return customers;
}

//Transaction Page
vector<RecentTransaction> InventoryManager::searchTransactions(string productName, string typeFilter) {
	sql::Connection* con = databaseManager.getConnection();

	string query =
		"SELECT st.transactionID, st.userID, u.userName, p.productName, c.customerFullName, "
		"st.transactionType, st.quantity, st.timeStamp "
		"FROM stockTransaction st "
		"JOIN product p ON p.productID = st.productID "
		"JOIN users u ON u.userID = st.userID "
		"LEFT JOIN customers c ON c.customerID = st.customerID "
		"WHERE p.productName LIKE ?";

	if (typeFilter != "")
		query += " AND st.transactionType = ?";

	sql::PreparedStatement* stmt =
		con->prepareStatement(query);

	string keyword = "%" + productName + "%";

	stmt->setString(1, keyword);

	if (typeFilter != "")
		stmt->setString(2, typeFilter);

	sql::ResultSet* res = stmt->executeQuery();

	vector<RecentTransaction> transactions;

	while (res->next())
	{
		RecentTransaction transaction;

		transaction.transactionID =
			res->getInt("transactionID");

		transaction.userID =
			res->getInt("userID");

		transaction.userName =
			res->getString("userName");

		transaction.productName =
			res->getString("productName");

		transaction.customerName =
			res->getString("customerFullName");

		transaction.type =
			Transaction::stringToTransactionType(
				res->getString("transactionType"));

		transaction.quantity =
			res->getInt("quantity");

		transaction.timeStamp =
			res->getString("timeStamp");
		
		transactions.push_back(transaction);
	}

	delete res;
	delete stmt;

	return transactions;
}

//Reports Page
int InventoryManager::getTransactionCount() {
	try {
		sql::Connection* con = databaseManager.getConnection();

		sql::PreparedStatement* stmt =
			con->prepareStatement(
				"SELECT COUNT(*) "
				"FROM stockTransaction"
			);
		sql::ResultSet* res = stmt->executeQuery();

		int transactionCount = 0;

		if (res->next())
			transactionCount = res->getInt(1);

		delete res;
		delete stmt;

		return transactionCount;
	}
	catch (sql::SQLException& e) {
		cout << "SQL Error: " << e.what() << endl;
		return 0;
	}
}

vector<InventorySummaryReport> InventoryManager::getAllInventorySummaryReport() {
	sql::Connection* con = databaseManager.getConnection();

	sql::PreparedStatement* stmt =
		con->prepareStatement(
			"SELECT p.productID, p.productName, c.categoryName, p.currentStock, "
					"p.price, (p.currentStock * p.price) AS totalValue "
			"FROM product p "
			"JOIN category c ON p.categoryID = c.categoryID"
		);
	sql::ResultSet* res = stmt->executeQuery();

	vector<InventorySummaryReport> inventoryReports;

	while (res->next())
	{
		InventorySummaryReport inventoryReport;
		
		inventoryReport.productID = res->getInt("productID");
		inventoryReport.productName = res->getString("productName");
		inventoryReport.categoryName = res->getString("categoryName");
		inventoryReport.currentStock = res->getInt("currentStock");

		inventoryReport.unitPrice = res->getDouble("price");
		inventoryReport.totalValue = res->getDouble("totalValue");

		inventoryReports.push_back(inventoryReport);
	}

	delete res;
	delete stmt;

	return inventoryReports;
}

vector<TransactionReport> InventoryManager::getAllTransactionReport() {
	sql::Connection* con = databaseManager.getConnection();

	sql::PreparedStatement* stmt =
		con->prepareStatement(
			"SELECT st.transactionID, p.productName, c.customerFullName, u.userName, st.transactionType, "
					"st.quantity, st.timeStamp "
			"FROM stockTransaction st "
			"JOIN product p ON st.productID = p.productID "
			"JOIN customers c ON st.customerID = c.customerID "
			"JOIN users u ON st.userID = u.userID"
		);
	sql::ResultSet* res = stmt->executeQuery();

	vector<TransactionReport> transactionReports;

	while (res->next())
	{
		TransactionReport transactionReport;

		transactionReport.transactionID = res->getInt("transactionID");
		transactionReport.productName = res->getString("productName");
		transactionReport.customerName = res->getString("customerFullName");
		transactionReport.userName = res->getString("userName");
		transactionReport.transactionType = res->getString("transactionType");
		transactionReport.quantity = res->getInt("quantity");
		transactionReport.transactionDate = res->getString("timeStamp");

		transactionReports.push_back(transactionReport);
	}

	delete res;
	delete stmt;

	return transactionReports;
}

vector<ProductReport> InventoryManager::getAllProductReport() {
	sql::Connection* con = databaseManager.getConnection();

	sql::PreparedStatement* stmt =
		con->prepareStatement(
			"SELECT p.productID, p.productName, "
			"SUM(CASE WHEN st.transactionType = 'PURCHASE' THEN st.quantity ELSE 0 END) AS purchased, "
			"SUM(CASE WHEN st.transactionType = 'PRODUCTION' THEN st.quantity ELSE 0 END) AS produced, "
			"SUM(CASE WHEN st.transactionType = 'SALE' THEN st.quantity ELSE 0 END) AS sold, "
			"SUM(CASE WHEN st.transactionType = 'USAGE' THEN st.quantity ELSE 0 END) AS used, "
			"SUM(CASE WHEN st.transactionType = 'LOSS' THEN st.quantity ELSE 0 END) AS lost, "
			"SUM(CASE WHEN st.transactionType = 'RETURN ACCEPTED' THEN st.quantity ELSE 0 END) AS returned, "
			"p.currentStock "
			"FROM product p "
			"LEFT JOIN stockTransaction st ON p.productID = st.productID "
			"GROUP BY p.productID, p.productName, p.currentStock"
		);
	sql::ResultSet* res = stmt->executeQuery();

	vector<ProductReport> productReports;

	while (res->next())
	{
		ProductReport productReport;

		productReport.productID = res->getInt("productID");
		productReport.productName = res->getString("productName");
		productReport.purchased = res->getInt("purchased");
		productReport.produced = res->getInt("produced");
		productReport.sold = res->getInt("sold");
		productReport.used = res->getInt("used");
		productReport.lost = res->getInt("lost");
		productReport.returned = res->getInt("returned");
		productReport.currentStock = res->getInt("currentStock");

		productReports.push_back(productReport);
	}

	delete res;
	delete stmt;

	return productReports;
}

vector<CustomerReport> InventoryManager::getAllCustomerReport() {
	sql::Connection* con = databaseManager.getConnection();

	sql::PreparedStatement* stmt =
		con->prepareStatement(
			"SELECT c.customerID, c.customerFullName AS customerName, "
			"SUM(CASE WHEN st.transactionType = 'SALE' THEN st.quantity ELSE 0 END) AS totalPurchases, "
			"SUM(CASE WHEN st.transactionType = 'RETURN_ACCEPTED' THEN st.quantity ELSE 0 END) AS totalReturns, "
			"MAX(st.timeStamp) AS lastTransactionDate "
			"FROM customers c "
			"LEFT JOIN stockTransaction st ON c.customerID = st.customerID "
			"GROUP BY c.customerID, c.customerFullName"
		);
	sql::ResultSet* res = stmt->executeQuery();

	vector<CustomerReport> customerReports;

	while (res->next())
	{
		CustomerReport customerReport;

		customerReport.customerID = res->getInt("customerID");
		customerReport.customerName = res->getString("customerName");
		customerReport.totalPurchases = res->getInt("totalPurchases");
		customerReport.totalReturns = res->getInt("totalReturns");
		customerReport.lastTransactionDate = res->getString("lastTransactionDate");

		customerReports.push_back(customerReport);
	}

	delete res;
	delete stmt;

	return customerReports;
}

//UsersPage
vector<UserInfo> InventoryManager::getAllUsersDB() {
	sql::Connection* con = databaseManager.getConnection();

	sql::PreparedStatement* stmt =
		con->prepareStatement(
			"SELECT userID, userName, userRole, createdAt "
			"FROM users "
			"WHERE isActive = TRUE"
		);
	sql::ResultSet* res = stmt->executeQuery();

	vector<UserInfo> users;

	while (res->next())
	{
		UserInfo user;

		user.userID = res->getInt("userID");
		user.username = res->getString("userName");
		user.role = User::stringToRoleType(res->getString("userRole"));
		user.createdAt = res->getString("createdAt");

		users.push_back(user);
	}

	delete res;
	delete stmt;

	return users;
}

vector<UserInfo> InventoryManager::searchUserDB(string keyword) {
	sql::Connection* con = databaseManager.getConnection();

	string query =
		"SELECT userID, userName, userRole, createdAt "
		"FROM users "
		"WHERE userName LIKE ?";

	sql::PreparedStatement* stmt =
		con->prepareStatement(query);

	string userNameKeyword = "%" + keyword + "%";

	stmt->setString(1, userNameKeyword);

	sql::ResultSet* res = stmt->executeQuery();

	vector<UserInfo> users;

	while (res->next())
	{
		UserInfo user;

		user.userID = res->getInt("userID");
		user.username = res->getString("userName");
		user.role = User::stringToRoleType(res->getString("userRole"));
		user.createdAt = res->getString("createdAt");

		users.push_back(user);
	}

	delete res;
	delete stmt;

	return users;
}

bool InventoryManager::addUserDB(string userName, string password, RoleType role) {
	try {
		sql::Connection* con = databaseManager.getConnection();

		sql::PreparedStatement* stmt = con->prepareStatement(
			"INSERT into users "
			"(userName, userRole, userPassword) "
			"VALUES (?, ?, ?)"
		);
		string userRole = User::roleTypeToString(role);

		stmt->setString(1, userName);
		stmt->setString(2, userRole);
		stmt->setString(3, password);

		stmt->execute();

		delete stmt;
		return true;
	}
	catch (sql::SQLException& e) {
		QMessageBox::critical(
			nullptr,
			"SQL Error",
			QString::fromStdString(e.what())
		);
		return false;
	}
}

bool InventoryManager::hasTransaction(int userID) {
	try{
		sql::Connection* con = databaseManager.getConnection();

		sql::PreparedStatement* stmt =
			con->prepareStatement(
				"SELECT COUNT(*) "
				"FROM stockTransaction "
				"WHERE userID = ?"
			);

		stmt->setInt(1, userID);

		sql::ResultSet* res = stmt->executeQuery();

		bool result = false;

		if (res->next())
			result = res->getInt(1) > 0;

		delete res;
		delete stmt;

		return result;
	}
	catch (sql::SQLException& e){
		QMessageBox::critical(
			nullptr,
			"SQL Error",
			QString::fromStdString(e.what())
		);
		return false;
	}
}

bool InventoryManager::deactivateUser(int userID) {
	try{
		sql::Connection* con = databaseManager.getConnection();

		sql::PreparedStatement* pstmt = con->prepareStatement(
			"UPDATE users "
			"SET isActive = FALSE "
			"WHERE userID = ?"
		);

		pstmt->setInt(1, userID);

		int affectedRows = pstmt->executeUpdate();

		delete pstmt;
		return affectedRows > 0;
	}
	catch (sql::SQLException& e)
	{
		QMessageBox::critical(
			nullptr,
			"SQL Error",
			QString::fromStdString(e.what())
		);

		return false;
	}
}

bool InventoryManager::permanentlyDeleteUser(int userID) {
	try {
		sql::Connection* con = databaseManager.getConnection();

		sql::PreparedStatement* pstmt = con->prepareStatement(
			"DELETE FROM users "
			"WHERE userID = ?"
		);

		pstmt->setInt(1, userID);

		int affectedRows = pstmt->executeUpdate();

		delete pstmt;
		return affectedRows > 0;
	}
	catch (sql::SQLException& e)
	{
		QMessageBox::critical(
			nullptr,
			"SQL Error",
			QString::fromStdString(e.what())
		);

		return false;
	}
}

bool InventoryManager::deleteUserDB(int userID) {
	if (hasTransaction(userID))
		return deactivateUser(userID);
	else
		return permanentlyDeleteUser(userID);
}

//StockManagementPage
vector<ProductInfo> InventoryManager::getAllProductsInfoDB() {
	sql::Connection* con = databaseManager.getConnection();

	sql::PreparedStatement* stmt =
		con->prepareStatement(
			"SELECT p.productID, p.productName, p.currentStock, p.price, c.categoryName, c.categoryID "
			"FROM product p "
			"JOIN category c ON c.categoryID = p.categoryID "
		);
	sql::ResultSet* res = stmt->executeQuery();

	vector<ProductInfo> products;

	while (res->next())
	{
		ProductInfo product;

		product.productID = res->getInt("productID");
		product.categoryID = res->getInt("categoryID");

		product.productName = res->getString("productName");
		product.currentStock = res->getInt("currentStock");
		product.price = res->getDouble("price");
		product.categoryName = res->getString("categoryName");

		products.push_back(product);
	}

	delete res;
	delete stmt;

	return products;
}