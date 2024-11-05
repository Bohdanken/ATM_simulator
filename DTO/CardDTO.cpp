// CardDTO.cpp
#include "CardDTO.h"

CardDTO::CardDTO() : _id(0), _accountId(0), _number(0), _pin(0) {}

CardDTO::CardDTO(const CardEntity& entity)
    : _id(entity.getId()), _accountId(entity.getAccountId()), _number(entity.getNumber()), _pin(entity.getPin()) {}

CardDTO::CardDTO(const CardDTO& other)
    : _id(other._id), _accountId(other._accountId), _number(other._number), _pin(other._pin) {}

CardDTO& CardDTO::operator=(const CardDTO& other) {
    if (this != &other) {
        _id = other._id;
        _accountId = other._accountId;
        _number = other._number;
        _pin = other._pin;
    }
    return *this;
}

CardDTO::CardDTO(CardDTO&& other) noexcept
    : _id(other._id), _accountId(other._accountId), _number(other._number), _pin(other._pin) {
    other._id = 0;
    other._accountId = 0;
    other._number = 0;
    other._pin = 0;
}

CardDTO& CardDTO::operator=(CardDTO&& other) noexcept {
    if (this != &other) {
        _id = other._id;
        _accountId = other._accountId;
        _number = other._number;
        _pin = other._pin;

        other._id = 0;
        other._accountId = 0;
        other._number = 0;
        other._pin = 0;
    }
    return *this;
}

uint64_t CardDTO::getId() const { return _id; }
uint64_t CardDTO::getAccountId() const { return _accountId; }
int64_t CardDTO::getNumber() const { return _number; }
uint16_t CardDTO::getPin() const { return _pin; }

void CardDTO::setId(uint64_t id) { _id = id; }
void CardDTO::setAccountId(uint64_t accountId) { _accountId = accountId; }
void CardDTO::setNumber(int64_t number) { _number = number; }
void CardDTO::setPin(uint16_t pin) { _pin = pin; }
