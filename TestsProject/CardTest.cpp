#include "mocks/AccountMocks.h"
#include "mocks/CardMocks.h"

#include "utils/assert_utils.h"

#pragma region Tests implementation
/* Create */
void testCanCreateCard()
{
    // Create an account
    AccountDTO accountSource;
    accountSource.setNumber(532);
    accountSource.setBalance(2000);
    accountSource.setUserName("Oleksiy");
    const AccountDTO &accountResult(AccountService::create(accountSource));
    assert(AccountRepository::getById(accountResult.getId()).has_value());

    // Create a card for the account
    CardDTO cardSource;
    cardSource.setAccountId(accountResult.getId());
    cardSource.setPin(4621);
    const CardDTO &cardResult(CardService::create(cardSource));
    assert(CardRepository::getById(cardResult.getId()).has_value());
}

void testCannotCreateCardForInvalidAccount()
{
    // Create a card for an account that does not exist
    CardDTO cardSource;
    cardSource.setAccountId(392);
    cardSource.setPin(2993);
    assert_fail(CardService::create(cardSource));
}

void testCannotCreateDuplicateCardForAccount()
{
    // Create an account
    AccountDTO accountSource;
    accountSource.setNumber(532);
    accountSource.setBalance(2000);
    accountSource.setUserName("Oleksiy");
    const AccountDTO &accountResult(AccountService::create(accountSource));
    assert(AccountRepository::getById(accountResult.getId()).has_value());

    // Create a card for the account
    CardDTO cardSource1;
    cardSource1.setAccountId(accountResult.getId());
    cardSource1.setPin(4621);
    const CardDTO &cardResult(CardService::create(cardSource1));
    assert(CardRepository::getById(cardResult.getId()).has_value());

    // Attempt to create a second card for the same account
    CardDTO cardSource2;
    cardSource2.setAccountId(accountResult.getId());
    cardSource2.setPin(1540);
    assert_fail(CardService::create(cardSource2));
}

void testCannotCreateCardWithInvalidPin()
{
    // Create an account
    AccountDTO accountSource;
    accountSource.setNumber(532);
    accountSource.setBalance(2000);
    accountSource.setUserName("Oleksiy");
    const AccountDTO &accountResult(AccountService::create(accountSource));
    assert(AccountRepository::getById(accountResult.getId()).has_value());

    // Create a card for the account
    CardDTO cardSource;
    cardSource.setAccountId(accountResult.getId());
    cardSource.setPin(83483);
    assert_fail(CardService::create(cardSource));
}

/* Update */
void testCanUpdateCard()
{
    constexpr uint16_t expectedPin = 4312;

    // Create account 1
    AccountDTO accountSource1;
    accountSource1.setNumber(532);
    accountSource1.setBalance(2000);
    accountSource1.setUserName("Oleksiy");
    const AccountDTO &accountResult1(AccountService::create(accountSource1));
    assert(AccountRepository::getById(accountResult1.getId()).has_value());

    // Create account 2
    AccountDTO accountSource2;
    accountSource2.setNumber(992);
    accountSource2.setBalance(8800);
    accountSource2.setUserName("Michael");
    const AccountDTO &accountResult2(AccountService::create(accountSource2));
    assert(AccountRepository::getById(accountResult2.getId()).has_value());

    // Create a card for the first account
    CardDTO cardSource1;
    cardSource1.setAccountId(accountResult1.getId());
    cardSource1.setPin(4621);
    const CardDTO &cardResult1(CardService::create(cardSource1));
    assert(CardRepository::getById(cardResult1.getId()).has_value());

    // Update the card to be for the second account
    CardDTO cardSource2;
    cardSource2.setAccountId(accountResult2.getId());
    cardSource2.setPin(expectedPin);

    CardService::update(cardResult1.getId(), cardSource2);
    const CardDTO &cardResult2(CardService::getById(cardResult1.getId()));
    assert(cardResult2.getAccountId() == accountResult2.getId());
    assert(cardResult2.getPin() == expectedPin);
}

void testCannotUpdateCardBecomingForInvalidAccount()
{
    // ...
}

void testCannotUpdateCardBecomingDuplicateForAccount()
{
    // ...
}

void testCannotUpdateCardWithInvalidPin()
{
    // ...
}

/* Remove */
void testCanRemoveCard()
{
    // Create an account
    AccountDTO accountSource;
    accountSource.setNumber(532);
    accountSource.setBalance(2000);
    accountSource.setUserName("Oleksiy");
    const AccountDTO &accountResult(AccountService::create(accountSource));
    assert(AccountRepository::getById(accountResult.getId()).has_value());

    // Create a card for the account
    CardDTO cardSource;
    cardSource.setAccountId(accountResult.getId());
    cardSource.setPin(4621);
    const CardDTO &cardResult(CardService::create(cardSource));
    assert(CardRepository::getById(cardResult.getId()).has_value());

    // Remove the card
    CardService::remove(cardResult.getId());
    assert(!CardRepository::getById(cardResult.getId()).has_value());
}

void testCannotRemoveCardWithInvalidId()
{
    // Create an account
    AccountDTO accountSource;
    accountSource.setNumber(532);
    accountSource.setBalance(2000);
    accountSource.setUserName("Oleksiy");
    const AccountDTO &accountResult(AccountService::create(accountSource));
    assert(AccountRepository::getById(accountResult.getId()).has_value());

    // Create a card for the account
    CardDTO cardSource;
    cardSource.setAccountId(accountResult.getId());
    cardSource.setPin(4621);
    const CardDTO &cardResult(CardService::create(cardSource));
    assert(CardRepository::getById(cardResult.getId()).has_value());

    // Attempt to delete a card that doesn't exist
    assert_fail(CardService::remove(cardResult.getId() + 53));
}
#pragma endregion
