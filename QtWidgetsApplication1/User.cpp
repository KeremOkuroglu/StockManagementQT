#include "User.h"

User::User(int userID, string username, string userPassword,RoleType userRole) {
	this->userID = userID;
	this->userName = username;
	this->userRole = userRole;
	this->userPassword = userPassword;
}

RoleType User::getRoleType() const {
	return userRole;
}

string User::getUserName() const {
	return userName;
}

int User::getUserID() const {
	return userID;
}

string User::roleTypeToString(RoleType type) {
	switch (type) {
	case RoleType::ADMIN: return "ADMIN";
	case RoleType::EMPLOYEE: return "EMPLOYEE";
	default: return "UNKNOWN";
	}
}

RoleType User::stringToRoleType(const string& role) {
	if (role == "ADMIN")
		return RoleType::ADMIN;
	if (role == "EMPLOYEE")
		return RoleType::EMPLOYEE;
	throw invalid_argument("Invalid role type.");
}