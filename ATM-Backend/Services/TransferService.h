// TransferService.h
#pragma once
#include "../repositories/AccountRepository.h"
#include "TransferService.h"
#include "../DTO/TransferDTO.h"
#include "../repositories/TransferRepository.h"
#include <stdexcept>
#include <optional>

/**
 * @brief Service class for handling Transfer operations.
 */
class TransferService {
public:
    static void setRepository(TransferRepository& transferRepository, AccountRepository& accountRepository);
    static TransferDTO create(const TransferDTO& dto);
    static void update(uint64_t id, const TransferDTO& dto);
    static void remove(uint64_t id);
    static TransferDTO getById(uint64_t id);

private:
    inline static TransferRepository* transferRepository = nullptr;
    inline static AccountRepository* accountRepository = nullptr;
    static void validateCreate(const TransferDTO& dto);
    static void validateUpdate(uint64_t id, const TransferDTO& dto);
    static void validateAccountsAreDifferent(const TransferDTO& dto);
    static void validateDatetime(const TransferDTO& dto);
};
