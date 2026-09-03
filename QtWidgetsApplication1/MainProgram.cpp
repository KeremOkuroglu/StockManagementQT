#include "InventoryManager.h"

void showMenu();

int main() {
	InventoryManager inventoryManager;
	

	int userChoice = -1;

	while (userChoice != 0) {

		showMenu();

		cin >> userChoice;

		switch (userChoice) {
		case 17:
		{
			cin.ignore(numeric_limits<streamsize>::max(), '\n');

			string name;

			cout << "Category Name: ";
			getline(cin, name);

			inventoryManager.addCategoryDB(name);

			break;
		}
		case 18:
		{
			inventoryManager.listCategoriesDB();

			break;
		}
		case 19:
		{
			inventoryManager.listProductsDB();
			break;
		}
		case 20:
		{
			cin.ignore(numeric_limits<streamsize>::max(), '\n');

			string name;
			int categoryID;
			double price;

			cout << "Product Name: ";
			getline(cin, name);

			cout << "Category ID: ";
			cin >> categoryID;

			cout << "Price: ";
			cin >> price;

			inventoryManager.addProductDB(name, categoryID, price);

			break;
		}
		case 21: {
			int productID, quantity;
			TransactionType type = TransactionType::PURCHASE;
			cout << "Enter productID: ";
			cin >> productID;
			cout << "Enter quantity: ";
			cin >> quantity;
			bool success = inventoryManager.processTransactionDB(productID, quantity, type);
			if (success)
				cout << "Transaction successful\n";
			else
				cout << "Transaction failed\n";
			break;
		}
		case 22: {
			int productID, quantity;
			TransactionType type = TransactionType::SALE;
			cout << "Enter productID: ";
			cin >> productID;
			cout << "Enter quantity: ";
			cin >> quantity;
			bool success = inventoryManager.processTransactionDB(productID, quantity, type);
			if (success)
				cout << "Transaction successful\n";
			else
				cout << "Transaction failed\n";
			break;
		}
		case 30:
		{
			cin.ignore(numeric_limits<streamsize>::max(), '\n');

			string username;
			string password;
			int role;

			cout << "Username: ";
			getline(cin, username);

			cout << "Password: ";
			cin >> password;

			cout << "Role (0 = ADMIN, 1 = EMPLOYEE, 2 = CUSTOMER): ";
			cin >> role;

			if (inventoryManager.registerUser(
				username,
				password,
				static_cast<RoleType>(role)))
			{
				cout << "Register successful.\n";
			}
			else
			{
				cout << "Register failed.\n";
			}

			break;
		}
		case 31:
		{
			cin.ignore(numeric_limits<streamsize>::max(), '\n');

			string username;
			string password;

			cout << "Username: ";
			getline(cin, username);

			cout << "Password: ";
			cin >> password;

			if (inventoryManager.login(username, password))
			{
				cout << "Login successful.\n";
			}
			else
			{
				cout << "Login failed.\n";
			}

			break;
		}
		case 40:
		{
			int productID, quantity;

			cout << "Enter product ID: ";
			cin >> productID;

			cout << "Enter reserve quantity: ";
			cin >> quantity;

			if (inventoryManager.processTransactionDB(
				productID,
				quantity,
				TransactionType::RESERVE))
			{
				cout << "Reserve successful.\n";
			}
			else
			{
				cout << "Reserve failed.\n";
			}

			break;
		}
		case 41:
		{
			int productID, quantity;

			cout << "Enter product ID: ";
			cin >> productID;

			cout << "Enter return quantity: ";
			cin >> quantity;

			if (inventoryManager.processTransactionDB(
				productID,
				quantity,
				TransactionType::RETURN_REQUEST))
			{
				cout << "Return request created.\n";
			}
			else
			{
				cout << "Return request failed.\n";
			}

			break;
		}
		case 42:
		{
			int productID, quantity;

			cout << "Enter product ID: ";
			cin >> productID;

			cout << "Enter accepted quantity: ";
			cin >> quantity;

			if (inventoryManager.processTransactionDB(
				productID,
				quantity,
				TransactionType::RETURN_ACCEPTED))
			{
				cout << "Return accepted.\n";
			}
			else
			{
				cout << "Return accept failed.\n";
			}

			break;
		}
		case 43:
		{
			int productID, quantity;

			cout << "Enter product ID: ";
			cin >> productID;

			cout << "Enter rejected quantity: ";
			cin >> quantity;

			if (inventoryManager.processTransactionDB(
				productID,
				quantity,
				TransactionType::RETURN_REJECTED))
			{
				cout << "Return rejected.\n";
			}
			else
			{
				cout << "Return reject failed.\n";
			}

			break;
		}
		case 50:
		{
			cin.ignore(numeric_limits<streamsize>::max(), '\n');

			string fullName;
			string phone;
			string email;

			cout << "Customer Name: ";
			getline(cin, fullName);

			cout << "Phone: ";
			getline(cin, phone);

			cout << "Email: ";
			getline(cin, email);

			if (inventoryManager.addCustomerDB(fullName, phone, email))
				cout << "Customer added.\n";
			else
				cout << "Failed.\n";

			break;
		}
		case 51:
		{
			inventoryManager.listCustomers();
			break;
		}
		case 60:
		{
			int productID;
			int quantity;
			int customerID;

			cout << "Product ID: ";
			cin >> productID;

			cout << "Quantity: ";
			cin >> quantity;

			cout << "Customer ID: ";
			cin >> customerID;

			if (inventoryManager.processTransactionDB(
				productID,
				quantity,
				TransactionType::SALE,
				customerID))
			{
				cout << "Sale successful.\n";
			}
			else
			{
				cout << "Sale failed.\n";
			}

			break;
		}
		case 61:
		{
			int productID;
			int quantity;
			int customerID;

			cout << "Product ID: ";
			cin >> productID;

			cout << "Quantity: ";
			cin >> quantity;

			cout << "Customer ID: ";
			cin >> customerID;

			if (inventoryManager.processTransactionDB(
				productID,
				quantity,
				TransactionType::RETURN_REQUEST,
				customerID))
			{
				cout << "Return request created.\n";
			}
			else
			{
				cout << "Return request failed.\n";
			}

			break;
		}
		case 62:
		{
			int productID;
			int quantity;
			int customerID;

			cout << "Product ID: ";
			cin >> productID;

			cout << "Quantity: ";
			cin >> quantity;

			cout << "Customer ID: ";
			cin >> customerID;

			if (inventoryManager.processTransactionDB(
				productID,
				quantity,
				TransactionType::RETURN_ACCEPTED,
				customerID))
			{
				cout << "Return accepted.\n";
			}
			else
			{
				cout << "Return accept failed.\n";
			}

			break;
		}
		case 63:
		{
			int productID;
			int quantity;
			int customerID;

			cout << "Product ID: ";
			cin >> productID;

			cout << "Quantity: ";
			cin >> quantity;

			cout << "Customer ID: ";
			cin >> customerID;

			if (inventoryManager.processTransactionDB(
				productID,
				quantity,
				TransactionType::RETURN_REJECTED,
				customerID))
			{
				cout << "Return rejected.\n";
			}
			else
			{
				cout << "Return reject failed.\n";
			}

			break;
		}
		default:
			cout << "Unvalid number, try again..\n";
			break;
		}
		//cout << inventoryManager.getCurrentUser()->getUserID() << endl;
	}
}

void showMenu() {
	cout << "\n\n===== INVENTORY MANAGEMENT SYSTEM =====\n\n";

	cout << "0-) Exit\n\n";

	cout << "17-) Deneme SQL eklemesi Category kismina\n";
	cout << "18-) Deneme SQL yazdirmasi Category list metodu\n";
	cout << "19-) Deneme SQL yazdirmasi Product list metodu\n";
	cout << "20-) Deneme SQL eklemesi Product kismina\n";
	cout << "21-) Deneme SQL product arttýrmak\n";
	cout << "22-) Deneme SQL product azaltmak\n";

	cout << "\n30-) Deneme SQL Register User\n";
	cout << "31-) Deneme SQL Login User\n";

	cout << "\n40-) Deneme SQL Reserve Etme\n";
	cout << "41-) Deneme SQL Iade talebi olusturmak\n";
	cout << "42-) Deneme SQL Iade talebi onaylama\n";
	cout << "43-) Deneme SQL Iade Talebi reddetme\n";

	cout << "\n50-) Add Customer\n";
	cout << "51-) List Customers\n";

	cout << "\n60-) Sale (Customer)\n";
	cout << "61-) Return Request\n";
	cout << "62-) Return Accept\n";
	cout << "63-) Return Reject\n";


	cout << "=======================================\n";
	cout << "Enter your choice: ";
}