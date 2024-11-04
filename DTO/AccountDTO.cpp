// AccountDTO.cpp
#include "AccountDTO.h"

AccountDTO::AccountDTO() : _id(0), _number(0), _balance(0) {}

AccountDTO::AccountDTO(const AccountEntity& entity)
    : _id(entity.getId()), _number(entity.getNumber()), _balance(entity.getBalance()), _clientId(entity.getClientId()) {}

AccountDTO::AccountDTO(const AccountDTO& other)
    : _id(other._id), _number(other._number), _balance(other._balance), _clientId(other.getClientId()) {}

AccountDTO& AccountDTO::operator=(const AccountDTO& other) {
    if (this != &other) {
        _id = other.getId();
        _number = other.getNumber();
        _balance = other.getBalance();
        _clientId = other.getClientId();
    }
    return *this;
}

AccountDTO::AccountDTO(AccountDTO&& other) noexcept
    : _id(other._id), _number(other._number), _balance(other._balance), _clientId(std::move(other.getClientId())) {
    other._id = 0;
    other._number = 0;
    other._balance = 0;
}

AccountDTO& AccountDTO::operator=(AccountDTO&& other) noexcept {
    if (this != &other) {
        _id = other._id;
        _number = other._number;
        _balance = other._balance;
        _clientId = std::move(other.getClientId());

        other._id = 0;
        other._number = 0;
        other._balance = 0;
    }
    return *this;
}

uint64_t AccountDTO::getId() const { return _id; }
uint64_t AccountDTO::getNumber() const { return _number; }
uint64_t AccountDTO::getBalance() const { return _balance; }
uint64_t AccountDTO::getClientId() const { return _clientId; }

void AccountDTO::setId(uint64_t id) { _id = id; }
void AccountDTO::setNumber(uint64_t number) { _number = number; }
void AccountDTO::setBalance(uint64_t balance) { _balance = balance; }
void AccountDTO::setClientId(uint64_t clientId){ _clientId = clientId; }
