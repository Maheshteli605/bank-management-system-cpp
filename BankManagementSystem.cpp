/*
 * Bank Management Application
 * ----------------------------
 * A console-based C++ application that simulates core banking operations
 * using Object-Oriented Programming and file handling.
 *
 * Features:
 *   1. Create new account
 *   2. Deposit money
 *   3. Withdraw money
 *   4. Check balance
 *   5. Display all accounts
 *   6. Search account by account number
 *   0. Exit
 *
 * Customer data is stored persistently in "accounts.txt" using file I/O.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <limits>

using namespace std;

const string FILENAME = "accounts.txt";

// ---------- Account class (OOP) ----------
class Account {
private:
    int accNo;
    string name;
    string accType;
    double balance;

public:
    // Default constructor
    Account() : accNo(0), name(""), accType(""), balance(0.0) {}

    // Parameterized constructor
    Account(int accNo, string name, string accType, double balance) {
        this->accNo = accNo;
        this->name = name;
        this->accType = accType;
        this->balance = balance;
    }

    // ---------- Getters ----------
    int getAccNo() const { return accNo; }
    string getName() const { return name; }
    string getAccType() const { return accType; }
    double getBalance() const { return balance; }

    // ---------- Core banking operations ----------
    void deposit(double amount) {
        balance += amount;
    }

    bool withdraw(double amount) {
        if (amount > balance) {
            return false; // insufficient funds
        }
        balance -= amount;
        return true;
    }

    // ---------- Display a single account ----------
    void display() const {
        cout << left << setw(12) << accNo << setw(20) << name
             << setw(15) << accType << fixed << setprecision(2)
             << setw(12) << balance << "\n";
    }

    // ---------- Convert account to file line format ----------
    string toFileString() const {
        ostringstream oss;
        oss << accNo << "|" << name << "|" << accType << "|" << fixed << setprecision(2) << balance;
        return oss.str();
    }

    // ---------- Build account object from a file line ----------
    static Account fromFileString(const string& line) {
        stringstream ss(line);
        string accNoStr, name, accType, balanceStr;

        getline(ss, accNoStr, '|');
        getline(ss, name, '|');
        getline(ss, accType, '|');
        getline(ss, balanceStr, '|');

        return Account(stoi(accNoStr), name, accType, stod(balanceStr));
    }
};

// ---------- BankSystem class: manages the collection of accounts ----------
class BankSystem {
private:
    vector<Account> accounts;

    void loadFromFile() {
        accounts.clear();
        ifstream file(FILENAME);
        string line;

        if (!file) return; // No file yet on first run

        while (getline(file, line)) {
            if (line.empty()) continue;
            accounts.push_back(Account::fromFileString(line));
        }
        file.close();
    }

    void saveToFile() const {
        ofstream file(FILENAME, ios::trunc);
        for (const auto& acc : accounts) {
            file << acc.toFileString() << "\n";
        }
        file.close();
    }

public:
    BankSystem() {
        loadFromFile();
    }

    // ---------- Create Account ----------
    void createAccount() {
        int accNo;
        string name, accType;
        double initialDeposit;

        cout << "\n--- Create New Account ---\n";
        cout << "Enter Account Number: ";
        cin >> accNo;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        for (const auto& acc : accounts) {
            if (acc.getAccNo() == accNo) {
                cout << "An account with this number already exists!\n";
                return;
            }
        }

        cout << "Enter Account Holder Name: ";
        getline(cin, name);

        cout << "Enter Account Type (Savings/Current): ";
        getline(cin, accType);

        cout << "Enter Initial Deposit Amount: ";
        cin >> initialDeposit;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (initialDeposit < 0) {
            cout << "Initial deposit cannot be negative.\n";
            return;
        }

        Account newAcc(accNo, name, accType, initialDeposit);
        accounts.push_back(newAcc);
        saveToFile();

        cout << "\nAccount created successfully!\n";
    }

    // ---------- Deposit ----------
    void deposit() {
        int accNo;
        cout << "\nEnter Account Number: ";
        cin >> accNo;

        for (auto& acc : accounts) {
            if (acc.getAccNo() == accNo) {
                double amount;
                cout << "Enter amount to deposit: ";
                cin >> amount;

                if (amount <= 0) {
                    cout << "Deposit amount must be positive.\n";
                    return;
                }

                acc.deposit(amount);
                saveToFile();
                cout << "\nDeposit successful! New Balance: " << fixed
                     << setprecision(2) << acc.getBalance() << "\n";
                return;
            }
        }
        cout << "\nAccount not found.\n";
    }

    // ---------- Withdraw ----------
    void withdraw() {
        int accNo;
        cout << "\nEnter Account Number: ";
        cin >> accNo;

        for (auto& acc : accounts) {
            if (acc.getAccNo() == accNo) {
                double amount;
                cout << "Enter amount to withdraw: ";
                cin >> amount;

                if (amount <= 0) {
                    cout << "Withdrawal amount must be positive.\n";
                    return;
                }

                if (acc.withdraw(amount)) {
                    saveToFile();
                    cout << "\nWithdrawal successful! New Balance: " << fixed
                         << setprecision(2) << acc.getBalance() << "\n";
                } else {
                    cout << "\nInsufficient balance! Current Balance: " << fixed
                         << setprecision(2) << acc.getBalance() << "\n";
                }
                return;
            }
        }
        cout << "\nAccount not found.\n";
    }

    // ---------- Check Balance ----------
    void checkBalance() const {
        int accNo;
        cout << "\nEnter Account Number: ";
        cin >> accNo;

        for (const auto& acc : accounts) {
            if (acc.getAccNo() == accNo) {
                cout << "\nAccount Holder : " << acc.getName() << "\n";
                cout << "Account Type   : " << acc.getAccType() << "\n";
                cout << "Current Balance: " << fixed << setprecision(2)
                     << acc.getBalance() << "\n";
                return;
            }
        }
        cout << "\nAccount not found.\n";
    }

    // ---------- Display All Accounts ----------
    void displayAll() const {
        if (accounts.empty()) {
            cout << "\nNo accounts found.\n";
            return;
        }

        cout << "\n-------------------------------------------------------\n";
        cout << left << setw(12) << "Acc No" << setw(20) << "Name"
             << setw(15) << "Type" << setw(12) << "Balance" << "\n";
        cout << "-------------------------------------------------------\n";

        for (const auto& acc : accounts) {
            acc.display();
        }

        cout << "-------------------------------------------------------\n";
        cout << "Total Accounts: " << accounts.size() << "\n";
    }

    // ---------- Search Account ----------
    void searchAccount() const {
        int accNo;
        cout << "\nEnter Account Number to search: ";
        cin >> accNo;

        for (const auto& acc : accounts) {
            if (acc.getAccNo() == accNo) {
                cout << "\n--- Account Found ---\n";
                acc.display();
                return;
            }
        }
        cout << "\nAccount not found.\n";
    }
};

// ---------- Menu ----------
void showMenu() {
    cout << "\n=========================================================\n";
    cout << "              BANK MANAGEMENT APPLICATION (C++)          \n";
    cout << "=========================================================\n";
    cout << "1. Create New Account\n";
    cout << "2. Deposit Money\n";
    cout << "3. Withdraw Money\n";
    cout << "4. Check Balance\n";
    cout << "5. Display All Accounts\n";
    cout << "6. Search Account\n";
    cout << "0. Exit\n";
    cout << "=========================================================\n";
}

// ---------- Main ----------
int main() {
    BankSystem bank;
    int choice;

    do {
        showMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: bank.createAccount(); break;
            case 2: bank.deposit(); break;
            case 3: bank.withdraw(); break;
            case 4: bank.checkBalance(); break;
            case 5: bank.displayAll(); break;
            case 6: bank.searchAccount(); break;
            case 0: cout << "\nExiting... Thank you for banking with us!\n"; break;
            default: cout << "\nInvalid choice. Please try again.\n";
        }

    } while (choice != 0);

    return 0;
}
