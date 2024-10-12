#include "AccountTest.h"
#include <cassert>
#include <string>
struct AccountService;

struct AccountDTO {
    long long  id;
    long long number;
    std::string userName;
    long long balance;
};

void testCreateAccount() {
    AccountDTO createdAccount = AccountService::create(AccountDTO("name", 0,0));
    assert(createdAccount.getId() > 0);  

    AccountDTO newAccount = AccountService::getById(createdAccount.getId());
    assert(newAccount.getName() == "name");
    assert(newAccount.getBalance() == 0);
}

void testCreateAccountWithInvalidBalance() {
    AccountDTO createdAccount;
    bool result=false;
    try {
        createdAccount = AccountService::create(AccountDTO("name", -1,0));
    }
    catch(Exception e) {
        result = true;
    }
    assert(result);
}


void testCreateAccountWithInvalidName() {
    AccountDTO createdAccount;
    bool result = false;
    try {
        createdAccount = AccountService::create(AccountDTO("", 0,0));
    }
    catch (Exception e) {
        result = true;
    }
    assert(result);
}

void testDeposit() {
    AccountDTO createdAccount = AccountService::create(AccountDTO("name", 0, 0));
    assert(createdAccount.getId() > 0);
    int value = 100;
    bool transactionResult = AccountService::deposit(createdAccount, value);
    assert(transactionResult);
    AccountDTO newAccount = AccountService::getById(createdAccount.getId());
    assert(newAccount.getBalance() == (createdAccount.getBalance()+value));
}

void testWithdrawSufficientFunds() {
    AccountDTO createdAccount = AccountService::create(AccountDTO("name", 100, 0));
    assert(createdAccount.getId() > 0);
    int value = 99;
    bool transactionResult = AccountService::withdraw(createdAccount, value);
    assert(transactionResult);
    AccountDTO newAccount = AccountService::getById(createdAccount.getId());
    assert(newAccount.getBalance() == (createdAccount.getBalance()-value));
}

void testWithdrawInsufficientFunds() {
    AccountDTO createdAccount = AccountService::create(AccountDTO("name", 100,0));
    assert(createdAccount.getId() > 0);
    int value = 101;
    bool transactionResult = AccountService::withdraw(createdAccount, value);
    assert(!transactionResult);
}

void testCreateAccountWithDuplicateNumber() {
    AccountDTO firstAccount = AccountService::create(AccountDTO("user1", 0, 0));
    assert(firstAccount.getId() > 0);

    bool result = false;
    try {
        AccountDTO secondAccount = AccountService::create(AccountDTO("user2",0, firstAccount.getNumber()));
    }
    catch (Exception e) { 
        result = true;
    }
    assert(result);
}
