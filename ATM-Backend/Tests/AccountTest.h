#pragma once
#ifndef ACCOUNT_TEST_H
#define ACCOUNT_TEST_H

#pragma region Utils
void accountTestWrapper(void (*const)());
#pragma endregion

int test_main(int, char* []) {
    return 0;
}


#pragma region Tests declaration
void testCanCreateAccount();
void testCannotCreateAccountWithInvalidName();
void testCannotCreateAccountWithDuplicateNumber();

void testCanUpdateAccount();
void testCannotUpdateAccountWithInvalidName();
void testCannotUpdateAccountWithDuplicateNumber();

// TODO [T2]: Final service implementation must ensure no card depends on this account
void testCanRemoveAccount();
void testCannotRemoveAccountWithInvalidId();

void testCanDeposit();
void testCanWithdraw();
void testCannotOverdraft();

void deamonTestWorks();
#pragma endregion

inline void runAccountTestSuite()
{
    // Account creation tests
    accountTestWrapper(testCanCreateAccount);
    accountTestWrapper(testCannotCreateAccountWithInvalidName);
    accountTestWrapper(testCannotCreateAccountWithDuplicateNumber);

    // Account update tests
    accountTestWrapper(testCanUpdateAccount);
    accountTestWrapper(testCannotUpdateAccountWithInvalidName);
    accountTestWrapper(testCannotUpdateAccountWithDuplicateNumber);

    // Account deletion tests
    accountTestWrapper(testCanRemoveAccount);
    accountTestWrapper(testCannotRemoveAccountWithInvalidId);

    // Account deposit/withdraw operations tests
    accountTestWrapper(testCanDeposit);
    accountTestWrapper(testCanWithdraw);
    accountTestWrapper(testCannotOverdraft);

    deamonTestWorks();
}

#endif // !ACCOUNT_TEST_H
