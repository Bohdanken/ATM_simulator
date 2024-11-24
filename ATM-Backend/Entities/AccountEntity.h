// AccountEntity.h
#pragma once
#include <string>
#include <cstdint>

/**
 * @brief Represents an Account entity.
 */
class AccountEntity {
public:
    AccountEntity();
    AccountEntity(const AccountEntity& other);
    AccountEntity& operator=(const AccountEntity& other);
    AccountEntity(AccountEntity&& other) noexcept;
    AccountEntity& operator=(AccountEntity&& other) noexcept;

    uint64_t getId() const;
    uint64_t getNumber() const;
    uint64_t getBalance() const;
    uint64_t getClientId() const;

    void setId(uint64_t id);
    void setNumber(uint64_t number);
    void setBalance(uint64_t balance);
    void setClientId(uint64_t userName);

private:
    uint64_t _id;
    uint64_t _number;
    uint64_t _balance;
    uint64_t _clientId;
};
