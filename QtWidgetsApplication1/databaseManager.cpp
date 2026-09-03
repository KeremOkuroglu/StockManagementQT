#include "databaseManager.h"
#include "databaseConfig.h"

#include <iostream>
using namespace std;

DatabaseManager::DatabaseManager() {
	driver = nullptr;
	connection = nullptr;
}

DatabaseManager::~DatabaseManager() {
	disconnect();
}

bool DatabaseManager::connect() {

	try {
		driver = get_driver_instance();
		connection =
			driver->connect(
				DB_HOST,
				DB_USER,
				DB_PASSWORD
			);

		connection->setSchema(DB_SCHEMA);

		return true;
	}
	catch (sql::SQLException& e) {
		cout << "Connection Error: " << e.what() << endl;

		return false;
	}
}

void DatabaseManager::disconnect() {
	if (connection) {

		if (!connection->isClosed()) {
			connection->close();
		}
		
		delete connection;
		connection = nullptr;
	}
}

sql::Connection* DatabaseManager::getConnection() {
	return connection;
}