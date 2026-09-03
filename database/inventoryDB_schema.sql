CREATE database InventoryDB;

USE InventoryDB;


CREATE TABLE Category(
	categoryID INT PRIMARY KEY AUTO_INCREMENT,
    categoryName VARCHAR(100) NOT NULL UNIQUE
);


CREATE TABLE Product(
	productID INT PRIMARY KEY AUTO_INCREMENT,
    productName VARCHAR(100) NOT NULL UNIQUE,
    currentStock INT NOT NULL DEFAULT 0,
    price DECIMAL(10,2) NOT NULL,
    
    categoryID INT NOT NULL,
    FOREIGN KEY(categoryID) REFERENCES Category(categoryID)
);


CREATE TABLE Users(
	userID INT PRIMARY KEY AUTO_INCREMENT,
    userName VARCHAR(256) NOT NULL UNIQUE,
    userRole VARCHAR(20) NOT NULL,
    userPassword VARCHAR(30) NOT NULL,
    
    createdAt TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    isActive BOOLEAN NOT NULL DEFAULT TRUE
);


CREATE TABLE Customers (
    customerID INT PRIMARY KEY AUTO_INCREMENT,
    customerFullName VARCHAR(100) NOT NULL,
    Phone VARCHAR(20) NOT NULL,
    email VARCHAR(100) NOT NULL,
    createdAt DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP
);


CREATE TABLE StockTransaction (
    transactionID INT PRIMARY KEY AUTO_INCREMENT,
    transactionType VARCHAR(50) NOT NULL,
    quantity INT NOT NULL,
    timeStamp DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,

    productID INT NOT NULL,
    userID INT NOT NULL,
    customerID INT NULL,

    FOREIGN KEY (productID) REFERENCES Product(productID),
    FOREIGN KEY (userID) REFERENCES Users(userID),
    FOREIGN KEY (customerID) REFERENCES Customers(customerID)
);

INSERT INTO users (userName, userPassword, userRole)
VALUES ('admin', 'admin123', 'ADMIN');