#pragma once
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>

class DatabaseManager {
private:
	sql::Driver* driver;
	sql::Connection* connection;

public:
	DatabaseManager();
	~DatabaseManager();

	bool connect();
	void disconnect();

	sql::Connection* getConnection();
};