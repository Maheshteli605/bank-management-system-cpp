# Bank Management Application (C++)

A console-based Bank Management Application built in C++ using Object-Oriented Programming
and file handling, simulating core banking operations.

## Features
- Create new bank account
- Deposit money
- Withdraw money (with insufficient balance check)
- Check account balance
- Display all accounts in a formatted table
- Search account by account number
- Persistent storage using file I/O (accounts.txt)

## OOP Concepts Used
- Class & Objects: Account class encapsulates account data and behavior
- Encapsulation: private data members with public getter methods
- Constructors: default and parameterized constructors for Account
- Separation of concerns: BankSystem class manages the collection of accounts and file I/O independently from the Account class itself

## How to Compile & Run
g++ -std=c++17 -Wall BankManagementSystem.cpp -o bank
./bank

## Author
Mahesh Basavaraj Teli — C++ Programming Intern @ Thiranex (Intern ID: THX-JUL2326-304)
