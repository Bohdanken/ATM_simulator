// main.cpp
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
#include <iostream>
#include <string>
#include <optional>
#include <stdexcept>

// Helper function to print separators
void printSeparator() {
    std::cout << "--------------------------------------------------\n";
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

        // Test Cases

        printSeparator();
        std::cout << "Test 1: Create Client\n";
        ClientDTO newClient("john_oe", "John Doe", "john@example.com");
        ClientDTO createdClient = ClientService::create(newClient);
        std::cout << "Client created with ID: " << createdClient.getId() << "\n";

        printSeparator();
        std::cout << "Test 2: Create Account for Client\n";
        AccountDTO newAccount(1234567890, 1000, createdClient.getId());
        AccountDTO createdAccount = AccountService::create(newAccount);
        std::cout << "Account created with ID: " << createdAccount.getId() << "\n";

        printSeparator();
        std::cout << "Test 3: Create Card for Account\n";
        CardDTO newCard(createdAccount.getId(), 987654321, 1234);
        CardDTO createdCard = CardService::create(newCard);
        std::cout << "Card created with ID: " << createdCard.getId() << "\n";

        printSeparator();
        std::cout << "Test 4: View Account Details\n";
        AccountDTO fetchedAccount = AccountService::getById(createdAccount.getId());
        std::cout << "Account ID: " << fetchedAccount.getId() << "\n";
        std::cout << "Number: " << fetchedAccount.getNumber() << "\n";
        std::cout << "Balance: " << fetchedAccount.getBalance() << "\n";
        std::cout << "Client ID: " << fetchedAccount.getClientId() << "\n";

        printSeparator();
        std::cout << "Test 5: Deposit Funds\n";
        AccountService::deposit(createdAccount.getId(), 500);
        AccountDTO afterDeposit = AccountService::getById(createdAccount.getId());
        std::cout << "New Balance after Deposit: " << afterDeposit.getBalance() << "\n";

        printSeparator();
        std::cout << "Test 6: Withdraw Funds\n";
        AccountService::withdraw(createdAccount.getId(), 300);
        AccountDTO afterWithdrawal = AccountService::getById(createdAccount.getId());
        std::cout << "New Balance after Withdrawal: " << afterWithdrawal.getBalance() << "\n";

        printSeparator();
        std::cout << "Test 7: Update Account\n";
        AccountDTO updateAccount = afterWithdrawal;
        updateAccount.setBalance(1500);
        AccountService::update(createdAccount.getId(), updateAccount);
        AccountDTO updatedAccount = AccountService::getById(createdAccount.getId());
        std::cout << "Account Balance after Update: " << updatedAccount.getBalance() << "\n";

        printSeparator();
        std::cout << "Test 8: View Client Details\n";
        ClientDTO fetchedClient = ClientService::getById(createdClient.getId());
        std::cout << "Client ID: " << fetchedClient.getId() << "\n";
        std::cout << "Username: " << fetchedClient.getUserName() << "\n";
        std::cout << "Name: " << fetchedClient.getName() << "\n";
        std::cout << "Email: " << fetchedClient.getEmail() << "\n";

        printSeparator();
        std::cout << "Test 9: Update Client\n";
        ClientDTO updateClient = fetchedClient;
        updateClient.setEmail("john.doe@example.com");
        ClientService::update(createdClient.getId(), updateClient);
        ClientDTO updatedClient = ClientService::getById(createdClient.getId());
        std::cout << "Client Email after Update: " << updatedClient.getEmail() << "\n";

        printSeparator();
        std::cout << "Test 10: View Card Details\n";
        CardDTO fetchedCard = CardService::getById(createdCard.getId());
        std::cout << "Card ID: " << fetchedCard.getId() << "\n";
        std::cout << "Account ID: " << fetchedCard.getAccountId() << "\n";
        std::cout << "Number: " << fetchedCard.getNumber() << "\n";
        std::cout << "PIN: " << fetchedCard.getPin() << "\n";

        printSeparator();
        std::cout << "Test 11: Update Card\n";
        CardDTO updateCard = fetchedCard;
        updateCard.setPin(4321);
        CardService::update(createdCard.getId(), updateCard);
        CardDTO updatedCard = CardService::getById(createdCard.getId());
        std::cout << "Card PIN after Update: " << updatedCard.getPin() << "\n";

        printSeparator();
        std::cout << "Test 12: Create Transfer\n";
        // Create another account for transfer
        AccountDTO secondAccount(1234567891, 2000, createdClient.getId());
        AccountDTO createdSecondAccount = AccountService::create(secondAccount);
        std::cout << "Second Account created with ID: " << createdSecondAccount.getId() << "\n";

        TransferDTO newTransfer(createdAccount.getId(), createdSecondAccount.getId(), 250, DateTime::getCurrent());
        TransferDTO createdTransfer = TransferService::create(newTransfer);
        std::cout << "Transfer created with ID: " << createdTransfer.getId() << "\n";

        // Verify balances after transfer
        AccountDTO fromAccount = AccountService::getById(createdAccount.getId());
        AccountDTO toAccount = AccountService::getById(createdSecondAccount.getId());
        std::cout << "From Account Balance after Transfer: " << fromAccount.getBalance() << "\n";
        std::cout << "To Account Balance after Transfer: " << toAccount.getBalance() << "\n";

        printSeparator();
        std::cout << "Test 13: Update Transfer\n";
        TransferDTO updateTransfer = createdTransfer;
        updateTransfer.setAmount(300);
        TransferService::update(createdTransfer.getId(), updateTransfer);
        TransferDTO updatedTransfer = TransferService::getById(createdTransfer.getId());
        std::cout << "Transfer Amount after Update: " << updatedTransfer.getAmount() << "\n";

        printSeparator();
        std::cout << "Test 14: Remove Transfer\n";
        TransferService::remove(createdTransfer.getId());
        std::cout << "Transfer removed successfully.\n";

        // Verify balances after transfer removal (if applicable)
        // Assuming removal reverts the transfer
        fromAccount = AccountService::getById(createdAccount.getId());
        toAccount = AccountService::getById(createdSecondAccount.getId());
        std::cout << "From Account Balance after Transfer Removal: " << fromAccount.getBalance() << "\n";
        std::cout << "To Account Balance after Transfer Removal: " << toAccount.getBalance() << "\n";

        printSeparator();
        std::cout << "Test 15: Remove Card\n";
        CardService::remove(createdCard.getId());
        std::cout << "Card removed successfully.\n";

        printSeparator();
        std::cout << "Test 16: Remove Account\n";
        AccountService::remove(createdSecondAccount.getId());
        std::cout << "Second Account removed successfully.\n";

        printSeparator();
        std::cout << "Test 17: Error Handling - Create Duplicate Account\n";
        try {
            AccountDTO duplicateAccount(1234567890, 500, createdClient.getId()); // Same number as first account
            AccountService::create(duplicateAccount);
        }
        catch (const std::exception& e) {
            std::cerr << "Expected Error: " << e.what() << "\n";
        }

        printSeparator();
        std::cout << "Test 18: Error Handling - Withdraw More Than Balance\n";
        try {
            AccountService::withdraw(createdAccount.getId(), 2000); // Balance is 1500
        }
        catch (const std::exception& e) {
            std::cerr << "Expected Error: " << e.what() << "\n";
        }

        printSeparator();
        std::cout << "Test 19: Error Handling - Transfer from Non-Existing Account\n";
        try {
            TransferDTO invalidTransfer(9999999999, createdAccount.getId(), 100, DateTime::getCurrent()); // From account doesn't exist
            TransferService::create(invalidTransfer);
        }
        catch (const std::exception& e) {
            std::cerr << "Expected Error: " << e.what() << "\n";
        }

        printSeparator();
        std::cout << "All tests completed.\n";

    }
    catch (const std::exception& e) {
        std::cerr << "Unhandled Error: " << e.what() << "\n";
    }

    return 0;
}
