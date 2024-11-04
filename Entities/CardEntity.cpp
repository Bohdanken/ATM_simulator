// CardEntity.cpp
#include "CardEntity.h"

CardEntity::CardEntity() : _id(0), _accountId(0), _number(0), _pin(0) {}

CardEntity::CardEntity(const CardEntity& other)
    : _id(other._id), _accountId(other._accountId), _number(other._number), _pin(other._pin) {}

CardEntity& CardEntity::operator=(const CardEntity& other) {
    if (this != &other) {
        _id = other._id;
        _accountId = other._accountId;
        _number = other._number;
        _pin = other._pin;
    }
    return *this;
}

CardEntity::CardEntity(CardEntity&& other) noexcept
    : _id(other._id), _accountId(other._accountId), _number(other._number), _pin(other._pin) {
    other._id = 0;
    other._accountId = 0;
    other._number = 0;
    other._pin = 0;
}

CardEntity& CardEntity::operator=(CardEntity&& other) noexcept {
    if (this != &other) {
        _id = other.getId();
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

uint64_t CardEntity::getId() const { return _id; }
uint64_t CardEntity::getAccountId() const { return _accountId; }
int64_t CardEntity::getNumber() const { return _number; }
uint16_t CardEntity::getPin() const { return _pin; }

void CardEntity::setId(uint64_t id) { _id = id; }
void CardEntity::setAccountId(uint64_t accountId) { _accountId = accountId; }
void CardEntity::setNumber(int64_t number) { _number = number; }
void CardEntity::setPin(uint16_t pin) { _pin = pin; }
