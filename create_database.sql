-- create_database.sql

-- Create the 'car' table
CREATE TABLE IF NOT EXISTS car (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    registration_number TEXT NOT NULL UNIQUE,
    brand TEXT NOT NULL,
    model TEXT NOT NULL,
    modYear INTEGER NOT NULL,
    gear TEXT NOT NULL,
    fuelType TEXT NOT NULL,
    price INTEGER NOT NULL,
    status TEXT NOT NULL
);

-- Create the 'customer' table
CREATE TABLE IF NOT EXISTS customer (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    telNum TEXT NOT NULL,
    email TEXT NOT NULL,
    address TEXT NOT NULL
);

-- Create the 'rent' table
CREATE TABLE IF NOT EXISTS rent (
    rentId INTEGER PRIMARY KEY AUTOINCREMENT,
    customerId INTEGER NOT NULL,
    carId INTEGER NOT NULL,
    startDate TEXT NOT NULL,
    endDate TEXT NOT NULL,
    FOREIGN KEY(customerId) REFERENCES customer(id),
    FOREIGN KEY(carId) REFERENCES car(id)
);
