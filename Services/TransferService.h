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
    static TransferDTO create(const TransferDTO& dto);
    static void update(uint64_t id, const TransferDTO& dto);
    static void remove(uint64_t id);
    static TransferDTO getById(uint64_t id);

private:
    static TransferRepository transferRepository;
    static AccountRepository accountRepository;
    static void validateCreate(const TransferDTO& dto);
    static void validateUpdate(uint64_t id, const TransferDTO& dto);
    static void validateAccountsAreDifferent(const TransferDTO& dto);
    static void validateDatetime(const TransferDTO& dto);
};
