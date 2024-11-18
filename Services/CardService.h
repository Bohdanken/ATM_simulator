// CardService.h
#pragma once
#include "../repositories/AccountRepository.h"
#include "../repositories/CardRepository.h"
#include <stdexcept>
#include <optional>

/**
 * @brief Service class for handling Card operations.
 */
class CardService {
public:
    static void setRepository(CardRepository& cardRepository, AccountRepository& accountRepository);
    static CardDTO create(const CardDTO& dto);
    static void update(uint64_t id, const CardDTO& dto);
    static void remove(uint64_t id);
    static CardDTO getById(uint64_t id);
    static CardDTO getByNumber(long long number);

private:
    inline static CardRepository* cardRepository = nullptr;
    inline static AccountRepository* accountRepository = nullptr;
    static void validateCreate(const CardDTO& dto);
    static void validateUpdate(uint64_t id, const CardDTO& dto);
    static void validatePin(const CardDTO& dto);

};
