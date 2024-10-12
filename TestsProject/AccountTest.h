#pragma once
#ifndef ACCOUNT_TEST_H
#define ACCOUNT_TEST_H

void testCreateAccount();
void testCreateAccountWithInvalidBalance();
void testCreateAccountWithInvalidName();

// TODO: test update, test delete 

void testDeposit();
void testWithdrawSufficientFunds();
void testWithdrawInsufficientFunds();
void testCreateAccountWithDuplicateNumber();

#endif // !ACCOUNT_TEST_H
