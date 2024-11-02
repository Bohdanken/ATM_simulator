#include "repositories/AccountRepository.h"
#include <iostream>

int main() {
    try {
        std::string connectionStr = "dbname=ATMSimulator user=postgres password=29072004";
        AccountRepository accountRepo(connectionStr);

        accountRepo.clear();
        std::cout << "All accounts cleared.\n";

        // Test: Add a new account
        AccountEntity newAccount{ 1, "12345678", 1000.0, "John Doe" }; 
        accountRepo.save(newAccount);
        std::cout << "New account saved.\n";

        // Test: Retrieve account by ID
        auto retrievedAccount = accountRepo.getById(1);
        if (retrievedAccount) {
            std::cout << "Account retrieved: " << retrievedAccount->userName << "\n";
        }
        else {
            std::cout << "Account not found.\n";
        }

        // Test: Update account
        AccountDTO updatedAccount{ "87654321", 2000.0, "Jane Doe" };
        accountRepo.update(1, updatedAccount);
        std::cout << "Account updated.\n";

        // Verify update
        retrievedAccount = accountRepo.getById(1);
        if (retrievedAccount) {
            std::cout << "Updated Account: " << retrievedAccount->userName << ", Balance: " << retrievedAccount->balance << "\n";
        }

        // Test: Remove account
        accountRepo.remove(1);
        std::cout << "Account removed.\n";

        // Verify removal
        retrievedAccount = accountRepo.getById(1);
        if (!retrievedAccount) {
            std::cout << "Account successfully deleted.\n";
        }
    }
    catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << "\n";
    }

    return 0;
}
