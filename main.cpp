// atm.cpp
#define _WIN32_WINNT 0x0601 // Targeting Windows 7

#include "repositories/AccountRepository.h"
#include "repositories/ClientRepository.h"
#include "repositories/CardRepository.h"
#include "repositories/TransferRepository.h"
#include "services/AccountService.h"
#include "services/ClientService.h"
#include "services/CardService.h"
#include "services/TransferService.h"
#include "DTO/AccountDTO.h"
#include "DTO/ClientDTO.h"
#include "DTO/CardDTO.h"
#include "DTO/TransferDTO.h"
#include "utils/time_utils.h"
#include "Entities/TransferDaemon.cpp" // Include the TransferDaemon header
#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <optional>
#include <stdexcept>
#include <limits>
#include <thread>
#include <vector>
#include <memory>

// Helper function to print separators
void printSeparator() {
    std::cout << "--------------------------------------------------\n";
}

// Function to clear input buffer
void clearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Function to authenticate user via card number and PIN
std::optional<CardDTO> authenticate() {
    long long cardNumber;
    int pin;
    std::cout << "Enter Card Number: ";
    while (!(std::cin >> cardNumber)) {
        std::cout << "Invalid input. Please enter a valid card number: ";
        std::cin.clear();
        clearInputBuffer();
    }
    std::cout << "Enter PIN: ";
    while (!(std::cin >> pin)) {
        std::cout << "Invalid input. Please enter a valid PIN: ";
        std::cin.clear();
        clearInputBuffer();
    }

    try {
        CardDTO card = CardService::getByNumber(cardNumber);
        if (card.getPin() == pin) {
            return card;
        }
        else {
            std::cout << "Incorrect PIN.\n";
            return std::nullopt;
        }
    }
    catch (const std::exception& e) {
        std::cout << "Authentication failed: " << e.what() << "\n";
        return std::nullopt;
    }
}

void displayMenu() {
    std::cout << "\nATM Menu:\n";
    std::cout << "1. Check Balance\n";
    std::cout << "2. Withdraw Funds\n";
    std::cout << "3. Deposit Funds\n";
    std::cout << "4. Transfer Funds\n";
    std::cout << "5. View Account Details\n";
    std::cout << "6. Set Up Scheduled Transfer\n"; // New option
    std::cout << "7. Exit\n";
    std::cout << "Select an option: ";
}

int main() {
    try {
        // Initialize repositories with your actual connection string
        AccountRepository accountRepo("host=localhost port=5432 dbname=ATMSimulator user=postgres password=29072004");
        ClientRepository clientRepo("host=localhost port=5432 dbname=ATMSimulator user=postgres password=29072004");
        CardRepository cardRepo("host=localhost port=5432 dbname=ATMSimulator user=postgres password=29072004");
        TransferRepository transferRepo("host=localhost port=5432 dbname=ATMSimulator user=postgres password=29072004");

        // Initialize services and set repositories
        AccountService::setRepository(accountRepo);
        ClientService::setRepository(clientRepo);
        CardService::setRepository(cardRepo, accountRepo);
        TransferService::setRepository(transferRepo, accountRepo);

        // Initialize Boost Asio I/O context
        boost::asio::io_context io;

        // Vector to hold shared pointers to TransferDaemon instances
        std::vector<std::shared_ptr<TransferDaemon>> daemons;

        // Run the I/O context in a separate thread
        std::thread ioThread([&io]() {
            io.run();
            });

        std::cout << "Welcome to the ATM.\n";
        printSeparator();

        while (true) {
            auto cardOpt = authenticate();
            if (!cardOpt.has_value()) {
                std::cout << "Authentication failed. Try again.\n";
                continue;
            }
            CardDTO card = cardOpt.value();
            AccountDTO account = AccountService::getById(card.getAccountId());
            ClientDTO client = ClientService::getById(account.getClientId());

            std::cout << "Welcome, " << client.getName() << "!\n";

            bool exit = false;
            while (!exit) {
                displayMenu();
                int choice;
                while (!(std::cin >> choice)) {
                    std::cout << "Invalid input. Please select a valid option: ";
                    std::cin.clear();
                    clearInputBuffer();
                }
                switch (choice) {
                case 1: { // Check Balance
                    account = AccountService::getById(account.getId()); // Refresh account data
                    std::cout << "Your current balance is: " << account.getBalance() << "\n";
                    break;
                }
                case 2: { // Withdraw Funds
                    std::cout << "Enter amount to withdraw: ";
                    double amount;
                    while (!(std::cin >> amount) || amount <= 0) {
                        std::cout << "Invalid amount. Please enter a positive number: ";
                        std::cin.clear();
                        clearInputBuffer();
                    }
                    try {
                        AccountService::withdraw(account.getId(), amount);
                        std::cout << "Withdrawal successful.\n";
                    }
                    catch (const std::exception& e) {
                        std::cout << "Error: " << e.what() << "\n";
                    }
                    break;
                }
                case 3: { // Deposit Funds
                    std::cout << "Enter amount to deposit: ";
                    double amount;
                    while (!(std::cin >> amount) || amount <= 0) {
                        std::cout << "Invalid amount. Please enter a positive number: ";
                        std::cin.clear();
                        clearInputBuffer();
                    }
                    try {
                        AccountService::deposit(account.getId(), amount);
                        std::cout << "Deposit successful.\n";
                    }
                    catch (const std::exception& e) {
                        std::cout << "Error: " << e.what() << "\n";
                    }
                    break;
                }
                case 4: { // Transfer Funds
                    std::cout << "Enter recipient account number: ";
                    long long recipientAccountNumber;
                    while (!(std::cin >> recipientAccountNumber)) {
                        std::cout << "Invalid input. Please enter a valid account number: ";
                        std::cin.clear();
                        clearInputBuffer();
                    }
                    std::cout << "Enter amount to transfer: ";
                    double amount;
                    while (!(std::cin >> amount) || amount <= 0) {
                        std::cout << "Invalid amount. Please enter a positive number: ";
                        std::cin.clear();
                        clearInputBuffer();
                    }
                    try {
                        AccountDTO recipientAccount = AccountService::getByNumber(recipientAccountNumber);
                        TransferDTO transfer(account.getId(), recipientAccount.getId(), amount, DateTime::getCurrent());
                        TransferService::create(transfer);
                        std::cout << "Transfer successful.\n";
                    }
                    catch (const std::exception& e) {
                        std::cout << "Error: " << e.what() << "\n";
                    }
                    break;
                }
                case 5: { // View Account Details
                    account = AccountService::getById(account.getId()); // Refresh account data
                    std::cout << "Account Details:\n";
                    std::cout << "Account ID: " << account.getId() << "\n";
                    std::cout << "Account Number: " << account.getNumber() << "\n";
                    std::cout << "Balance: " << account.getBalance() << "\n";
                    std::cout << "Client ID: " << account.getClientId() << "\n";
                    break;
                }
                case 6: { // Set Up Scheduled Transfer
                    std::cout << "Enter recipient account number: ";
                    long long recipientAccountNumber;
                    while (!(std::cin >> recipientAccountNumber)) {
                        std::cout << "Invalid input. Please enter a valid account number: ";
                        std::cin.clear();
                        clearInputBuffer();
                    }
                    std::cout << "Enter amount to transfer each time: ";
                    double amount;
                    while (!(std::cin >> amount) || amount <= 0) {
                        std::cout << "Invalid amount. Please enter a positive number: ";
                        std::cin.clear();
                        clearInputBuffer();
                    }
                    std::cout << "Enter frequency in seconds: ";
                    int freqSeconds;
                    while (!(std::cin >> freqSeconds) || freqSeconds <= 0) {
                        std::cout << "Invalid frequency. Please enter a positive integer: ";
                        std::cin.clear();
                        clearInputBuffer();
                    }
                    try {
                        AccountDTO recipientAccount = AccountService::getByNumber(recipientAccountNumber);

                        // Define the transfer condition (always true in this case)
                        auto transferCondition = []() -> bool {
                            return true;
                        };

                        // Create a shared pointer to the TransferDaemon and add it to the vector
                        auto daemon = std::make_shared<TransferDaemon>(
                            io,
                            account.getId(),
                            recipientAccount.getId(),
                            std::chrono::seconds(freqSeconds),
                            amount,
                            transferCondition
                            );
                        daemons.push_back(daemon);

                        std::cout << "Scheduled transfer set up successfully.\n";
                    }
                    catch (const std::exception& e) {
                        std::cout << "Error: " << e.what() << "\n";
                    }
                    break;
                }
                case 7: { // Exit
                    std::cout << "Thank you for using the ATM. Goodbye!\n";
                    exit = true;
                    break;
                }
                default:
                    std::cout << "Invalid choice. Please try again.\n";
                    break;
                }
            }
            // Optionally allow another user to log in
            std::cout << "Would you like to perform another transaction? (y/n): ";
            char again;
            std::cin >> again;
            if (again == 'n' || again == 'N') {
                break;
            }
        }

        // Stop the I/O context and join the thread
        io.stop();
        if (ioThread.joinable()) {
            ioThread.join();
        }

    }
    catch (const std::exception& e) {
        std::cerr << "Unhandled Error: " << e.what() << "\n";
    }

    return 0;
}
