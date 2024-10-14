#include "utils/assert_utils.h"
#include <string>
#include <list>
#include <optional>
#include <stdexcept>
#include <cstdint>
#include <string>
#include <ctime>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>

using namespace std;

#define _WIN32_WINNT 0x0601

#pragma region Mock definitions
struct AccountEntity
{
    static inline uint64_t freeId = 0;
    /*const*/ uint64_t _id;
    uint64_t _number;
    uint64_t _balance;
    std::string _userName;

    inline uint64_t getId() const { return _id; }
    inline uint64_t getNumber() const { return _number; }
    inline uint64_t getBalance() const { return _balance; }
    inline const std::string getUserName() const { return _userName; }

    inline void setNumber(const uint64_t number) { _number = number; }
    inline void setBalance(const uint64_t balance) { _balance = balance; }
    inline void setUserName(const std::string& userName) { _userName = userName; }
};




struct AccountDTO
{
    AccountDTO() = default;

    AccountDTO(const AccountEntity& entity) :
        _id(entity.getId()),
        _number(entity.getNumber()),
        _balance(entity.getBalance()),
        _userName(entity.getUserName())
    {
    }

    uint64_t _id;
    uint64_t _number;
    uint64_t _balance;
    std::string _userName;

    inline uint64_t getId() const { return _id; }
    inline uint64_t getNumber() const { return _number; }
    inline uint64_t getBalance() const { return _balance; }
    inline const std::string getUserName() const { return _userName; }

    inline void setId(const uint64_t id) { _id = id; }
    inline void setNumber(const uint64_t number) { _number = number; }
    inline void setBalance(const uint64_t balance) { _balance = balance; }
    inline void setUserName(const std::string& userName) { _userName = userName; }
};

struct AccountRepository
{
    // For tests
    static inline void clear() { accounts.clear(); }

    static inline std::optional<AccountEntity> getById(const uint64_t id)
    {
        for (const AccountEntity& entity : accounts)
        {
            if (entity.getId() == id)
                return entity;
        }
        return std::nullopt;
    }

    static inline std::list<AccountEntity> getAll() { return accounts; }

    static inline void save(const AccountEntity& entity) { accounts.push_back(entity); }

    static inline void update(const uint64_t id, const AccountDTO& dto)
    {
        for (AccountEntity& entity : accounts)
        {
            if (entity.getId() == id)
            {
                entity.setNumber(dto.getNumber());
                entity.setBalance(dto.getBalance());
                entity.setUserName(dto.getUserName());
                break;
            }
        }
    }

    static inline void remove(const uint64_t id)
    {
        accounts.remove_if(
            [id](const AccountEntity& entity) -> bool { return entity.getId() == id; }
        );
    }

    // TODO: remove after db setup
    static inline std::list<AccountEntity> accounts;
};

struct AccountService
{
    static inline AccountDTO create(const AccountDTO& dto)
    {
        validateCreate(dto);
        AccountEntity entity;
        entity._id = entity.freeId++; // will be handled by the database
        entity.setNumber(dto.getNumber());
        entity.setBalance(dto.getBalance());
        entity.setUserName(dto.getUserName());
        AccountRepository::save(entity);
        return AccountDTO(entity);
    }

    static inline void update(const uint64_t id, const AccountDTO& dto)
    {
        validateUpdate(id, dto);
        AccountRepository::update(id, dto);
    }

    static inline void remove(const uint64_t id)
    {
        const std::optional<AccountEntity>& optEntity(AccountRepository::getById(id));
        if (!optEntity.has_value())
            throw std::invalid_argument("Error: Cannot delete an account that doesn't exist");
        AccountRepository::remove(id);
    }

    static inline AccountDTO getById(const uint64_t id)
    {
        const std::optional<AccountEntity>& optEntity(AccountRepository::getById(id));
        if (!optEntity.has_value())
            throw std::invalid_argument("Error: Cannot get a DTO of an account that does not exist");
        return AccountDTO(optEntity.value());
    }

    static inline void deposit(const uint64_t id, const uint64_t sum)
    {
        const std::optional<AccountEntity>& optEntity(AccountRepository::getById(id));
        if (!optEntity.has_value())
            throw std::invalid_argument("Error: Cannot deposit funds to an account that doesn't exist");
        AccountDTO dto(optEntity.value());
        dto.setBalance(dto.getBalance() + sum);
        AccountRepository::update(id, dto);
    }

    static inline void withdraw(const uint64_t id, const uint64_t sum)
    {
        const std::optional<AccountEntity>& optEntity(AccountRepository::getById(id));
        if (!optEntity.has_value())
            throw std::invalid_argument("Error: Cannot withdraw funds from an account that doesn't exist");

        AccountDTO dto(optEntity.value());
        if (dto.getBalance() < sum)
            throw std::invalid_argument("Error: Trying to withdraw more money than currently available");
        dto.setBalance(dto.getBalance() - sum);
        AccountRepository::update(id, dto);
    }

    static inline void validateCreate(const AccountDTO& dto)
    {
        validateName(dto);
        for (const AccountEntity& entity : AccountRepository::getAll())
        {
            if (entity.getNumber() == dto.getNumber())
                throw std::invalid_argument("Error: Account number must be unique");
        }
    }

    static inline void validateUpdate(const uint64_t id, const AccountDTO& dto)
    {
        const std::optional<AccountEntity>& optEntity(AccountRepository::getById(id));
        if (!optEntity.has_value())
            throw std::invalid_argument("Error: Cannot update an account that doesn't exist");

        validateName(dto);
        for (const AccountEntity& entity : AccountRepository::getAll())
        {
            if (entity.getNumber() == dto.getNumber() && entity.getId() != id)
                throw std::invalid_argument("Error: Cannot update to an account number that is already in use");
        }
    }

    static inline void validateName(const AccountDTO& dto)
    {
        if (dto.getUserName().empty())
            throw std::invalid_argument("Error: Account user name cannot be empty");
    }
};
#pragma endregion


struct TransferDaemon {
    AccountDTO* from;
    AccountDTO* to;
    boost::asio::steady_timer timer;
    std::chrono::seconds frequency;
    const double amount;
    std::function<bool()> condition;

    TransferDaemon(boost::asio::io_context& io, AccountDTO* fromAcc, AccountDTO* toAcc, std::chrono::seconds freq, double amt, std::function<bool()> cond)
        : from(fromAcc), to(toAcc), timer(io, freq), frequency(freq), amount(amt), condition(cond) {
        scheduleTransfer();
    }


    void scheduleTransfer() {
        timer.async_wait([this](const boost::system::error_code& ec) {
            if (!ec) {
                checkAndTransfer();
                timer.expires_at(timer.expiry() + frequency);
                scheduleTransfer();
            }
            else {
                std::cerr << "Error in timer: " << ec.message() << std::endl;
            }
            });
    }


    void checkAndTransfer() {
        if (condition()) {
            cout << "Yeah, code" << endl;
            //TODO define how to use Transaction
        }
    }
};


#pragma region DeamonTests implementation
void deamonTestWorks()
{
    boost::asio::io_context io;

    AccountDTO fromAccount= AccountDTO();
    AccountDTO toAccount= AccountDTO();
    fromAccount.setUserName("From");
    fromAccount.setBalance(500);
    toAccount.setUserName("To");
    toAccount.setBalance(100);

    auto transferCondition = [&fromAccount]() -> bool {
        return fromAccount.getBalance() > 200.0;
    };

    // Create a TransferDaemon that transfers 50 units every  second
    TransferDaemon daemon(io, &fromAccount, &toAccount, std::chrono::seconds(1), 50.0, transferCondition);
    io.run();
    std::this_thread::sleep_for(std::chrono::seconds(15));
    assert(toAccount.getBalance() == 600);

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

    const AccountDTO& resultDTO(AccountService::create(sourceDTO));
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

    const AccountDTO& resultDTO(AccountService::create(dto1));
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

    const AccountDTO& resultDTO(AccountService::create(sourceDTO));
    assert(AccountRepository::getById(resultDTO.getId()).has_value());

    AccountDTO newDTO;
    newDTO.setNumber(expectedNumber);
    newDTO.setBalance(expectedBalance);
    newDTO.setUserName(expectedUserName);

    AccountService::update(resultDTO.getId(), newDTO);
    const AccountDTO& result2DTO(AccountService::getById(resultDTO.getId()));
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

    const AccountDTO& resultDTO(AccountService::create(sourceDTO));
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

    const AccountDTO& resultDTO(AccountService::create(dto1));
    assert(AccountRepository::getById(resultDTO.getId()).has_value());

    const AccountDTO& result2DTO(AccountService::create(dto2));
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

    const AccountDTO& resultDTO(AccountService::create(sourceDTO));
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

    const AccountDTO& resultDTO(AccountService::create(sourceDTO));
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

    const AccountDTO& resultDTO(AccountService::create(sourceDTO));
    assert(AccountRepository::getById(resultDTO.getId()).has_value());

    AccountService::deposit(resultDTO.getId(), depositSum);
    const AccountDTO& result2DTO(AccountService::getById(resultDTO.getId()));
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

    const AccountDTO& resultDTO(AccountService::create(sourceDTO));
    assert(AccountRepository::getById(resultDTO.getId()).has_value());

    AccountService::withdraw(resultDTO.getId(), withdrawSum);
    const AccountDTO& result2DTO(AccountService::getById(resultDTO.getId()));
    assert(result2DTO.getBalance() == expectedBalance);
}

void testCannotOverdraft()
{
    constexpr uint64_t initialSum = 2000, withdrawSum = 3000;

    AccountDTO sourceDTO;
    sourceDTO.setNumber(532);
    sourceDTO.setBalance(initialSum);
    sourceDTO.setUserName("Oleksiy");

    const AccountDTO& resultDTO(AccountService::create(sourceDTO));
    assert(AccountRepository::getById(resultDTO.getId()).has_value());
    assert_fail(AccountService::withdraw(resultDTO.getId(), withdrawSum));
}
#pragma endregion

#pragma region Utils implementation
void accountTestWrapper(void (* const fn)())
{
    AccountRepository::clear();
    fn();
}
#pragma endregion





