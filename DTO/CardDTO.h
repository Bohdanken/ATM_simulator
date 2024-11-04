// CardDTO.h
#pragma once
#include <cstdint>
#include "../Entities/CardEntity.h"

/**
 * @brief Data Transfer Object for Card.
 */
class CardDTO {
public:
    CardDTO();
    CardDTO(const CardEntity& entity);
    CardDTO(const CardDTO& other);
    CardDTO& operator=(const CardDTO& other);
    CardDTO(CardDTO&& other) noexcept;
    CardDTO& operator=(CardDTO&& other) noexcept;

    uint64_t getId() const;
    uint64_t getAccountId() const;
    int64_t getNumber() const;
    uint16_t getPin() const;

    void setId(uint64_t id);
    void setAccountId(uint64_t accountId);
    void setNumber(int64_t number);
    void setPin(uint16_t pin);

private:
    uint64_t _id;
    uint64_t _accountId;
    int64_t _number;
    uint16_t _pin;
};
