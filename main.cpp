#include "repositories/AccountRepository.h"
#include <iostream>

int main() {
    try {
        // Connection string: replace with your actual database details
        std::string connectionStr = "host=localhost port=5432 dbname=ATMSimulator user=postgres password=29072004";

        // Instantiate the repository
        AccountRepository accountRepo(connectionStr);

        // Clear all accounts initially (optional for testing purposes)
        accountRepo.clear();
        std::cout << "Cleared all accounts.\n";

        // Test - Add a new account
        AccountEntity newAccount{ 0, 1, 1000005, 1000.50 }; // Adjust clientId, number, and balance as needed
        accountRepo.save(newAccount);
        std::cout << "Account added.\n";

        // Test - Retrieve all accounts and display them
        auto accounts = accountRepo.getAll();
        std::cout << "All accounts:\n";
        for (const auto& account : accounts) {
            std::cout << "ID: " << account.id
                << ", ClientID: " << account.clientId
                << ", Number: " << account.number
                << ", Balance: " << account.balance << '\n';
        }

        // Test - Retrieve account by ID
        auto retrievedAccount = accountRepo.getById(newAccount.id);
        if (retrievedAccount) {
            std::cout << "Retrieved account with ID " << retrievedAccount->id
                << ": Balance = " << retrievedAccount->balance << '\n';
        }
        else {
            std::cout << "Account not found.\n";
        }

        // Test - Update account
        AccountDTO updateData{ 1000005, 2000.75 };
        accountRepo.update(newAccount.id, updateData);
        std::cout << "Account updated.\n";

        // Verify the update
        retrievedAccount = accountRepo.getById(newAccount.id);
        if (retrievedAccount) {
            std::cout << "Updated account balance: " << retrievedAccount->balance << '\n';
        }

        // Test - Remove account
        accountRepo.remove(newAccount.id);
        std::cout << "Account removed.\n";

        // Verify removal
        retrievedAccount = accountRepo.getById(newAccount.id);
        if (!retrievedAccount) {
            std::cout << "Account successfully removed.\n";
        }
        else {
            std::cout << "Failed to remove account.\n";
        }
    }
    catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << '\n';
    }

    return 0;
}
