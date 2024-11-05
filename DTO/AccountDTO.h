// AccountDTO.h
#pragma once
#include <string>
#include <cstdint>
#include "../Entities/AccountEntity.h"

/**
 * @brief Data Transfer Object for Account.
 */
class AccountDTO {
public:
    AccountDTO();
    AccountDTO(const AccountEntity& entity);
    AccountDTO(const AccountDTO& other);
    AccountDTO& operator=(const AccountDTO& other);
    AccountDTO(AccountDTO&& other) noexcept;
    AccountDTO& operator=(AccountDTO&& other) noexcept;

    uint64_t getId() const;
    uint64_t getNumber() const;
    uint64_t getBalance() const;
    uint64_t getClientId() const;

    void setId(uint64_t id);
    void setNumber(uint64_t number);
    void setBalance(uint64_t balance);
    void setClientId(uint64_t clientId);

    uint64_t _id;
    uint64_t _number;
    uint64_t _balance;
    uint64_t _clientId;
};
