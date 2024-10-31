#include "mocks/TransferMocks.h"


#include "utils/assert_utils.h"

#pragma region Utils implementation
void trasferTestWrapper(void (*const fn)())
{
    AccountRepository::clear();
    TransferRepository::clear();
    fn();
}
#pragma endregion

#pragma region Tests implementation
/* Create */
void testCanCreateTransfer()
{
    // Create account 1
    AccountDTO accountSource1;
    accountSource1.setNumber(532);
    accountSource1.setBalance(2000);
    accountSource1.setUserName("Oleksiy");
    const AccountDTO &accountResult1(AccountService::create(accountSource1));
    assert(AccountRepository::getById(accountResult1.getId()).has_value());

    // Create account 2
    AccountDTO accountSource2;
    accountSource2.setNumber(53876);
    accountSource2.setBalance(92000);
    accountSource2.setUserName("Michael");
    const AccountDTO &accountResult2(AccountService::create(accountSource2));
    assert(AccountRepository::getById(accountResult2.getId()).has_value());

    // Create transfer
    TransferDTO transferSource;
    transferSource.setFromAccountId(accountResult1.getId());
    transferSource.setToAccountId(accountResult2.getId());
    transferSource.setAmount(500);
    // transferSource.setDateTime(DateTime::getCurrent()); -- already initialised this way

    const TransferDTO &transferResult(TransferService::create(transferSource));
    assert(TransferRepository::getById(transferResult.getId()).has_value());
}

void testCannotCreateTransferFromInvalidAccount()
{
    // Create account 1 (to)
    AccountDTO accountSource1;
    accountSource1.setNumber(532);
    accountSource1.setBalance(2000);
    accountSource1.setUserName("Oleksiy");
    const AccountDTO &accountResult1(AccountService::create(accountSource1));
    assert(AccountRepository::getById(accountResult1.getId()).has_value());

    // Attempt to create transfer from an invalid account
    TransferDTO transferSource;
    transferSource.setFromAccountId(accountResult1.getId() + 5443);
    transferSource.setToAccountId(accountResult1.getId());
    transferSource.setAmount(500);
    assert_fail(TransferService::create(transferSource));
}

void testCannotCreateTransferToInvalidAccount()
{
    // Create account 1 (from)
    AccountDTO accountSource1;
    accountSource1.setNumber(532);
    accountSource1.setBalance(2000);
    accountSource1.setUserName("Oleksiy");
    const AccountDTO &accountResult1(AccountService::create(accountSource1));
    assert(AccountRepository::getById(accountResult1.getId()).has_value());

    // Attempt to create transfer to an invalid account
    TransferDTO transferSource;
    transferSource.setFromAccountId(accountResult1.getId());
    transferSource.setToAccountId(accountResult1.getId() + 5443);
    transferSource.setAmount(500);
    assert_fail(TransferService::create(transferSource));
}

void testCannotCreateTransferForTheSameAccount()
{
    // Create account 1 (from & to)
    AccountDTO accountSource1;
    accountSource1.setNumber(532);
    accountSource1.setBalance(2000);
    accountSource1.setUserName("Oleksiy");
    const AccountDTO &accountResult1(AccountService::create(accountSource1));
    assert(AccountRepository::getById(accountResult1.getId()).has_value());

    // Attempt to create transfer for the same account
    TransferDTO transferSource;
    transferSource.setFromAccountId(accountResult1.getId());
    transferSource.setToAccountId(accountResult1.getId());
    transferSource.setAmount(500);
    assert_fail(TransferService::create(transferSource));
}

void testCannotCreateTransferInTheFuture()
{
    // Create bad datetime
    DateTime badDatetime(DateTime::getCurrent());
    ++badDatetime.second;

    // Create account 1
    AccountDTO accountSource1;
    accountSource1.setNumber(532);
    accountSource1.setBalance(2000);
    accountSource1.setUserName("Oleksiy");
    const AccountDTO &accountResult1(AccountService::create(accountSource1));
    assert(AccountRepository::getById(accountResult1.getId()).has_value());

    // Create account 2
    AccountDTO accountSource2;
    accountSource2.setNumber(53876);
    accountSource2.setBalance(92000);
    accountSource2.setUserName("Michael");
    const AccountDTO &accountResult2(AccountService::create(accountSource2));
    assert(AccountRepository::getById(accountResult2.getId()).has_value());
    
    // Attempt to create a transfer in the future
    TransferDTO transferSource;
    transferSource.setFromAccountId(accountResult1.getId());
    transferSource.setToAccountId(accountResult2.getId());
    transferSource.setAmount(500);
    transferSource.setDateTime(badDatetime);
    assert_fail(TransferService::create(transferSource));
}

void testCannotCreateTransferTooFarInThePast()
{
    // Get bad datetime
    DateTime badDatetime(DateTime::getCurrent());
    badDatetime.year -= 200;

    // Create account 1
    AccountDTO accountSource1;
    accountSource1.setNumber(532);
    accountSource1.setBalance(2000);
    accountSource1.setUserName("Oleksiy");
    const AccountDTO &accountResult1(AccountService::create(accountSource1));
    assert(AccountRepository::getById(accountResult1.getId()).has_value());

    // Create account 2
    AccountDTO accountSource2;
    accountSource2.setNumber(53876);
    accountSource2.setBalance(92000);
    accountSource2.setUserName("Michael");
    const AccountDTO &accountResult2(AccountService::create(accountSource2));
    assert(AccountRepository::getById(accountResult2.getId()).has_value());

    // Attempt to create a transfer in the future
    TransferDTO transferSource;
    transferSource.setFromAccountId(accountResult1.getId());
    transferSource.setToAccountId(accountResult2.getId());
    transferSource.setAmount(500);
    transferSource.setDateTime(badDatetime);
    assert_fail(TransferService::create(transferSource));
}

/* Update */
void testCanUpdateTransfer()
{
    // Get a different datetime & amount
    constexpr uint64_t expectedAmount = 1500;
    DateTime expectedDatetime(DateTime::getCurrent());
    --expectedDatetime.day;

    // Create account 1
    AccountDTO accountSource1;
    accountSource1.setNumber(532);
    accountSource1.setBalance(2000);
    accountSource1.setUserName("Oleksiy");
    const AccountDTO &accountResult1(AccountService::create(accountSource1));
    assert(AccountRepository::getById(accountResult1.getId()).has_value());

    // Create account 2
    AccountDTO accountSource2;
    accountSource2.setNumber(53876);
    accountSource2.setBalance(92000);
    accountSource2.setUserName("Michael");
    const AccountDTO &accountResult2(AccountService::create(accountSource2));
    assert(AccountRepository::getById(accountResult2.getId()).has_value());

    // Create a transfer
    TransferDTO transferSource1;
    transferSource1.setFromAccountId(accountResult1.getId());
    transferSource1.setToAccountId(accountResult2.getId());
    transferSource1.setAmount(500);
    const TransferDTO &transferResult1(TransferService::create(transferSource1));
    assert(TransferRepository::getById(transferResult1.getId()).has_value());

    // Update the transfer to have a different amount and a different datetime
    TransferDTO transferSource2;
    transferSource2.setFromAccountId(accountResult1.getId());
    transferSource2.setToAccountId(accountResult2.getId());
    transferSource2.setAmount(expectedAmount);
    transferSource2.setDateTime(expectedDatetime);

    TransferService::update(transferResult1.getId(), transferSource2);
    const TransferDTO &transferResult2(TransferService::getById(transferResult1.getId()));
    assert(transferResult2.getFromAccountId() == accountResult1.getId());
    assert(transferResult2.getToAccountId() == accountResult2.getId());
    assert(transferResult2.getAmount() == expectedAmount);
    assert(transferResult2.getDateTime() == expectedDatetime);
}

void testCannotUpdateTransferToBecomeFromInvalidAccount()
{
    // Create account 1
    AccountDTO accountSource1;
    accountSource1.setNumber(532);
    accountSource1.setBalance(2000);
    accountSource1.setUserName("Oleksiy");
    const AccountDTO &accountResult1(AccountService::create(accountSource1));
    assert(AccountRepository::getById(accountResult1.getId()).has_value());

    // Create account 2
    AccountDTO accountSource2;
    accountSource2.setNumber(53876);
    accountSource2.setBalance(92000);
    accountSource2.setUserName("Michael");
    const AccountDTO &accountResult2(AccountService::create(accountSource2));
    assert(AccountRepository::getById(accountResult2.getId()).has_value());

    // Create a transfer
    TransferDTO transferSource1;
    transferSource1.setFromAccountId(accountResult1.getId());
    transferSource1.setToAccountId(accountResult2.getId());
    transferSource1.setAmount(500);
    const TransferDTO &transferResult1(TransferService::create(transferSource1));
    assert(TransferRepository::getById(transferResult1.getId()).has_value());

    // Attempt to update the transfer to become from an invalid account
    TransferDTO transferSource2;
    transferSource2.setFromAccountId(accountResult1.getId() + 548);
    transferSource2.setToAccountId(accountResult2.getId());
    transferSource2.setAmount(500);
    assert_fail(TransferService::update(transferResult1.getId(), transferSource2));
}

void testCannotUpdateTransferToBecomeToInvalidAccount()
{
    // Create account 1
    AccountDTO accountSource1;
    accountSource1.setNumber(532);
    accountSource1.setBalance(2000);
    accountSource1.setUserName("Oleksiy");
    const AccountDTO &accountResult1(AccountService::create(accountSource1));
    assert(AccountRepository::getById(accountResult1.getId()).has_value());

    // Create account 2
    AccountDTO accountSource2;
    accountSource2.setNumber(53876);
    accountSource2.setBalance(92000);
    accountSource2.setUserName("Michael");
    const AccountDTO &accountResult2(AccountService::create(accountSource2));
    assert(AccountRepository::getById(accountResult2.getId()).has_value());

    // Create a transfer
    TransferDTO transferSource1;
    transferSource1.setFromAccountId(accountResult1.getId());
    transferSource1.setToAccountId(accountResult2.getId());
    transferSource1.setAmount(500);
    const TransferDTO &transferResult1(TransferService::create(transferSource1));
    assert(TransferRepository::getById(transferResult1.getId()).has_value());

    // Attempt to update the transfer to become to an invalid account
    TransferDTO transferSource2;
    transferSource2.setFromAccountId(accountResult1.getId());
    transferSource2.setToAccountId(accountResult2.getId() + 548);
    transferSource2.setAmount(500);
    assert_fail(TransferService::update(transferResult1.getId(), transferSource2));
}

void testCannotUpdateTransferToBecomeForTheSameAccount()
{
    // Create account 1
    AccountDTO accountSource1;
    accountSource1.setNumber(532);
    accountSource1.setBalance(2000);
    accountSource1.setUserName("Oleksiy");
    const AccountDTO &accountResult1(AccountService::create(accountSource1));
    assert(AccountRepository::getById(accountResult1.getId()).has_value());

    // Create account 2
    AccountDTO accountSource2;
    accountSource2.setNumber(53876);
    accountSource2.setBalance(92000);
    accountSource2.setUserName("Michael");
    const AccountDTO &accountResult2(AccountService::create(accountSource2));
    assert(AccountRepository::getById(accountResult2.getId()).has_value());

    // Create a transfer
    TransferDTO transferSource1;
    transferSource1.setFromAccountId(accountResult1.getId());
    transferSource1.setToAccountId(accountResult2.getId());
    transferSource1.setAmount(500);
    const TransferDTO &transferResult1(TransferService::create(transferSource1));
    assert(TransferRepository::getById(transferResult1.getId()).has_value());

    // Attempt to update the transfer to become for the same account
    TransferDTO transferSource2;
    transferSource2.setFromAccountId(accountResult1.getId());
    transferSource2.setToAccountId(accountResult1.getId());
    transferSource2.setAmount(500);
    assert_fail(TransferService::update(transferResult1.getId(), transferSource2));
}

void testCannotUpdateTransferToBecomeInTheFuture()
{
    // Get bad datetime
    DateTime badDatetime(DateTime::getCurrent());
    ++badDatetime.day;

    // Create account 1
    AccountDTO accountSource1;
    accountSource1.setNumber(532);
    accountSource1.setBalance(2000);
    accountSource1.setUserName("Oleksiy");
    const AccountDTO &accountResult1(AccountService::create(accountSource1));
    assert(AccountRepository::getById(accountResult1.getId()).has_value());

    // Create account 2
    AccountDTO accountSource2;
    accountSource2.setNumber(53876);
    accountSource2.setBalance(92000);
    accountSource2.setUserName("Michael");
    const AccountDTO &accountResult2(AccountService::create(accountSource2));
    assert(AccountRepository::getById(accountResult2.getId()).has_value());

    // Create a transfer
    TransferDTO transferSource1;
    transferSource1.setFromAccountId(accountResult1.getId());
    transferSource1.setToAccountId(accountResult2.getId());
    transferSource1.setAmount(500);
    const TransferDTO &transferResult1(TransferService::create(transferSource1));
    assert(TransferRepository::getById(transferResult1.getId()).has_value());

    // Attempt to update the transfer to become in the future
    TransferDTO transferSource2;
    transferSource2.setFromAccountId(accountResult1.getId());
    transferSource2.setToAccountId(accountResult2.getId());
    transferSource2.setAmount(500);
    transferSource2.setDateTime(badDatetime);
    assert_fail(TransferService::update(transferResult1.getId(), transferSource2));
}

void testCannotUpdateTransferToBecomeTooFarInThePast()
{
    // Get bad datetime
    DateTime badDatetime(DateTime::getCurrent());
    badDatetime.year -= 200;

    // Create account 1
    AccountDTO accountSource1;
    accountSource1.setNumber(532);
    accountSource1.setBalance(2000);
    accountSource1.setUserName("Oleksiy");
    const AccountDTO &accountResult1(AccountService::create(accountSource1));
    assert(AccountRepository::getById(accountResult1.getId()).has_value());

    // Create account 2
    AccountDTO accountSource2;
    accountSource2.setNumber(53876);
    accountSource2.setBalance(92000);
    accountSource2.setUserName("Michael");
    const AccountDTO &accountResult2(AccountService::create(accountSource2));
    assert(AccountRepository::getById(accountResult2.getId()).has_value());

    // Create a transfer
    TransferDTO transferSource1;
    transferSource1.setFromAccountId(accountResult1.getId());
    transferSource1.setToAccountId(accountResult2.getId());
    transferSource1.setAmount(500);
    const TransferDTO &transferResult1(TransferService::create(transferSource1));
    assert(TransferRepository::getById(transferResult1.getId()).has_value());

    // Attempt to update the transfer to become in the future
    TransferDTO transferSource2;
    transferSource2.setFromAccountId(accountResult1.getId());
    transferSource2.setToAccountId(accountResult2.getId());
    transferSource2.setAmount(500);
    transferSource2.setDateTime(badDatetime);
    assert_fail(TransferService::update(transferResult1.getId(), transferSource2));
}

/* Remove */
void testCanRemoveTransfer()
{
    // Create account 1
    AccountDTO accountSource1;
    accountSource1.setNumber(532);
    accountSource1.setBalance(2000);
    accountSource1.setUserName("Oleksiy");
    const AccountDTO &accountResult1(AccountService::create(accountSource1));
    assert(AccountRepository::getById(accountResult1.getId()).has_value());

    // Create account 2
    AccountDTO accountSource2;
    accountSource2.setNumber(53876);
    accountSource2.setBalance(92000);
    accountSource2.setUserName("Michael");
    const AccountDTO &accountResult2(AccountService::create(accountSource2));
    assert(AccountRepository::getById(accountResult2.getId()).has_value());

    // Create transfer
    TransferDTO transferSource;
    transferSource.setFromAccountId(accountResult1.getId());
    transferSource.setToAccountId(accountResult2.getId());
    transferSource.setAmount(500);
    const TransferDTO &transferResult(TransferService::create(transferSource));
    assert(TransferRepository::getById(transferResult.getId()).has_value());

    // Delete the transfer
    TransferService::remove(transferResult.getId());
    assert(!TransferRepository::getById(transferResult.getId()).has_value());
}

void testCannotRemoveTransferWithInvalidId()
{
    // Create account 1
    AccountDTO accountSource1;
    accountSource1.setNumber(532);
    accountSource1.setBalance(2000);
    accountSource1.setUserName("Oleksiy");
    const AccountDTO &accountResult1(AccountService::create(accountSource1));
    assert(AccountRepository::getById(accountResult1.getId()).has_value());

    // Create account 2
    AccountDTO accountSource2;
    accountSource2.setNumber(53876);
    accountSource2.setBalance(92000);
    accountSource2.setUserName("Michael");
    const AccountDTO &accountResult2(AccountService::create(accountSource2));
    assert(AccountRepository::getById(accountResult2.getId()).has_value());

    // Create transfer
    TransferDTO transferSource;
    transferSource.setFromAccountId(accountResult1.getId());
    transferSource.setToAccountId(accountResult2.getId());
    transferSource.setAmount(500);
    const TransferDTO &transferResult(TransferService::create(transferSource));
    assert(TransferRepository::getById(transferResult.getId()).has_value());

    // Attempt to delete a transfer that does not exist
    assert_fail(TransferService::remove(transferResult.getId() + 53));
}
#pragma endregion
