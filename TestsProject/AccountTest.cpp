// TODO [T2?->Release]: Replace mocks with final implementations
#include "mocks/AccountMocks.h"

#include "utils/assert_utils.h"

#pragma region Utils implementation
void accountTestWrapper(void (*const fn)())
{
    AccountRepository::clear();
    fn();
}
#pragma endregion

#pragma region Tests implementation
/* Create */
void testCanCreateAccount()
{
    AccountDTO sourceDTO;
    sourceDTO.setNumber(532);
    sourceDTO.setBalance(2000);
    sourceDTO.setUserName("Oleksiy");
 
    const AccountDTO &resultDTO(AccountService::create(sourceDTO));
    assert(AccountRepository::getById(resultDTO.getId()).has_value());
    assert(resultDTO.getNumber() == sourceDTO.getNumber());
    assert(resultDTO.getBalance() == sourceDTO.getBalance());
    assert(resultDTO.getUserName() == sourceDTO.getUserName());
}

void testCannotCreateAccountWithInvalidName()
{
    AccountDTO sourceDTO;
    sourceDTO.setNumber(234);
    sourceDTO.setBalance(40);
    sourceDTO.setUserName("");
    assert_fail(AccountService::create(sourceDTO));
}

void testCannotCreateAccountWithDuplicateNumber()
{
    AccountDTO dto1;           
    dto1.setNumber(532);
    dto1.setBalance(2000);
    dto1.setUserName("Oleksiy");

    AccountDTO dto2;
    dto2.setNumber(dto1.getNumber());
    dto2.setBalance(5000);
    dto2.setUserName("Michael");

    const AccountDTO &resultDTO(AccountService::create(dto1));
    assert(AccountRepository::getById(resultDTO.getId()).has_value());
    assert_fail(AccountService::create(dto2));
}

/* Update */
void testCanUpdateAccount()
{
    constexpr uint64_t expectedNumber = 10, expectedBalance = 100;
    const std::string expectedUserName("Michael");

    AccountDTO sourceDTO;
    sourceDTO.setNumber(532);
    sourceDTO.setBalance(2000);
    sourceDTO.setUserName("Oleksiy");

    const AccountDTO &resultDTO(AccountService::create(sourceDTO));
    assert(AccountRepository::getById(resultDTO.getId()).has_value());

    AccountDTO newDTO;
    newDTO.setNumber(expectedNumber);
    newDTO.setBalance(expectedBalance);
    newDTO.setUserName(expectedUserName);

    AccountService::update(resultDTO.getId(), newDTO);
    const AccountDTO &result2DTO(AccountService::getById(resultDTO.getId()));
    assert(result2DTO.getNumber() == expectedNumber);
    assert(result2DTO.getBalance() == expectedBalance);
    assert(result2DTO.getUserName() == expectedUserName);
}

void testCannotUpdateAccountWithInvalidName()
{
    AccountDTO sourceDTO;
    sourceDTO.setNumber(532);
    sourceDTO.setBalance(2000);
    sourceDTO.setUserName("Oleksiy");

    const AccountDTO &resultDTO(AccountService::create(sourceDTO));
    assert(AccountRepository::getById(resultDTO.getId()).has_value());

    AccountDTO newDTO(sourceDTO);
    newDTO.setUserName("");
    assert_fail(AccountService::update(resultDTO.getId(), newDTO));
}

void testCannotUpdateAccountWithDuplicateNumber()
{
    AccountDTO dto1;
    dto1.setNumber(532);
    dto1.setBalance(2000);
    dto1.setUserName("Oleksiy");

    AccountDTO dto2;
    dto2.setNumber(123);
    dto2.setBalance(5000);
    dto2.setUserName("Michael");

    const AccountDTO &resultDTO(AccountService::create(dto1));
    assert(AccountRepository::getById(resultDTO.getId()).has_value());

    const AccountDTO &result2DTO(AccountService::create(dto2));
    assert(AccountRepository::getById(result2DTO.getId()).has_value());

    AccountDTO newDTO(dto2);
    newDTO.setNumber(dto1.getNumber());
    assert_fail(AccountService::update(result2DTO.getId(), newDTO));
}

/* Remove */
void testCanRemoveAccount()
{
    AccountDTO sourceDTO;
    sourceDTO.setNumber(532);
    sourceDTO.setBalance(2000);
    sourceDTO.setUserName("Oleksiy");

    const AccountDTO &resultDTO(AccountService::create(sourceDTO));
    assert(AccountRepository::getById(resultDTO.getId()).has_value());

    AccountService::remove(resultDTO.getId());
    assert(!AccountRepository::getById(resultDTO.getId()).has_value());
}

void testCannotRemoveAccountWithInvalidId()
{
    AccountDTO sourceDTO;
    sourceDTO.setNumber(532);
    sourceDTO.setBalance(2000);
    sourceDTO.setUserName("Oleksiy");

    const AccountDTO &resultDTO(AccountService::create(sourceDTO));
    assert(AccountRepository::getById(resultDTO.getId()).has_value());
    assert_fail(AccountService::remove(resultDTO.getId() + 774));
}

/* Deposit/Withdraw */
void testCanDeposit()
{
    constexpr uint64_t initialSum = 2000, depositSum = 500;
    constexpr uint64_t expectedBalance = initialSum + depositSum;

    AccountDTO sourceDTO;
    sourceDTO.setNumber(532);
    sourceDTO.setBalance(initialSum);
    sourceDTO.setUserName("Oleksiy");

    const AccountDTO &resultDTO(AccountService::create(sourceDTO));
    assert(AccountRepository::getById(resultDTO.getId()).has_value());

    AccountService::deposit(resultDTO.getId(), depositSum);
    const AccountDTO &result2DTO(AccountService::getById(resultDTO.getId()));
    assert(result2DTO.getBalance() == expectedBalance);
}

void testCanWithdraw()
{
    constexpr uint64_t initialSum = 2000, withdrawSum = 500;
    constexpr uint64_t expectedBalance = initialSum - withdrawSum;

    AccountDTO sourceDTO;
    sourceDTO.setNumber(532);
    sourceDTO.setBalance(initialSum);
    sourceDTO.setUserName("Oleksiy");

    const AccountDTO &resultDTO(AccountService::create(sourceDTO));
    assert(AccountRepository::getById(resultDTO.getId()).has_value());
                                                   
    AccountService::withdraw(resultDTO.getId(), withdrawSum);
    const AccountDTO &result2DTO(AccountService::getById(resultDTO.getId()));
    assert(result2DTO.getBalance() == expectedBalance);
}

void testCannotOverdraft()
{
    constexpr uint64_t initialSum = 2000, withdrawSum = 3000;

    AccountDTO sourceDTO;
    sourceDTO.setNumber(532);
    sourceDTO.setBalance(initialSum);
    sourceDTO.setUserName("Oleksiy");

    const AccountDTO &resultDTO(AccountService::create(sourceDTO));
    assert(AccountRepository::getById(resultDTO.getId()).has_value());
    assert_fail(AccountService::withdraw(resultDTO.getId(), withdrawSum));
}
#pragma endregion
