#pragma once
#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

enum class RoleType {
	ADMIN,
	EMPLOYEE,
};

struct UserInfo
{
	int userID = 0;
	string username = "";
	/*string fullName = "";*/
	RoleType role = RoleType::EMPLOYEE;
	string createdAt = "";
	/*string lastLogin = "";*/
};

class User {
private:
	int userID;
	string userName;
	string userPassword;
	RoleType userRole;

public:
	User(int id, string username, string userPassword,RoleType userRole);

	RoleType getRoleType() const;
	string getUserName() const;
	int getUserID() const;

	static string roleTypeToString(RoleType type);
	static RoleType stringToRoleType(const string& role);
};