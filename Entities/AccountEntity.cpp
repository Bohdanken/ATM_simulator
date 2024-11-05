// AccountEntity.cpp
#include "AccountEntity.h"

AccountEntity::AccountEntity() : _id(0), _number(0), _balance(0) {}

AccountEntity::AccountEntity(const AccountEntity& other)
    : _id(other._id), _number(other._number), _balance(other._balance), _clientId(other._clientId) {}

AccountEntity& AccountEntity::operator=(const AccountEntity& other) {
    if (this != &other) {
        _id = other._id;
        _number = other._number;
        _balance = other._balance;
        _clientId = other._clientId;
    }
    return *this;
}

AccountEntity::AccountEntity(AccountEntity&& other) noexcept
    : _id(other._id), _number(other._number), _balance(other._balance), _clientId(std::move(other._clientId)) {
    other._id = 0;
    other._number = 0;
    other._balance = 0;
}

AccountEntity& AccountEntity::operator=(AccountEntity&& other) noexcept {
    if (this != &other) {
        _id = other._id;
        _number = other._number;
        _balance = other._balance;
        _clientId = std::move(other._clientId);

        other._id = 0;
        other._number = 0;
        other._balance = 0;
    }
    return *this;
}

uint64_t AccountEntity::getId() const { return _id; }
uint64_t AccountEntity::getNumber() const { return _number; }
uint64_t AccountEntity::getBalance() const { return _balance; }
uint64_t AccountEntity::getClientId() const { return _clientId; }

void AccountEntity::setId(uint64_t id) { _id = id; }
void AccountEntity::setNumber(uint64_t number) { _number = number; }
void AccountEntity::setBalance(uint64_t balance) { _balance = balance; }
void AccountEntity::setClientId(uint64_t clientId) { _clientId = clientId; }
