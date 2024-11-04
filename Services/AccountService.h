// AccountService.h
#pragma once
#include "../DTO/AccountDTO.h"
#include "../Entities/AccountEntity.h"
#include <stdexcept>
#include "../repositories/AccountRepository.h"
#include <optional>

/**
 * @brief Service class for handling Account operations.
 */
class AccountService {
public:
    //AccountService(const std::string& connectionStr);
    static AccountDTO create(const AccountDTO& dto);
    static void update(uint64_t id, const AccountDTO& dto);
    static void remove(uint64_t id);
    static AccountDTO getById(uint64_t id);
    static void deposit(uint64_t id, uint64_t sum);
    static void withdraw(uint64_t id, uint64_t sum);

private:
    static AccountRepository accountRepository;
    static void validateCreate(const AccountDTO& dto);
    static void validateUpdate(uint64_t id, const AccountDTO& dto);
    static void validateClientId(const AccountDTO& dto);
};
