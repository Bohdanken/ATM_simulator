// TransferEntity.cpp
#include "TransferEntity.h"

TransferEntity::TransferEntity()
    : _id(0), _fromAccountId(0), _toAccountId(0), _amount(0), _datetime(DateTime::getCurrent()) {}

TransferEntity::TransferEntity(TransferEntity&& other) noexcept
    : _id(other._id),
    _fromAccountId(other._fromAccountId),
    _toAccountId(other._toAccountId),
    _amount(other._amount),
    _datetime(std::move(other._datetime)) {
    other._id = 0;
    other._fromAccountId = 0;
    other._toAccountId = 0;
    other._amount = 0;
}

TransferEntity& TransferEntity::operator=(TransferEntity&& other) noexcept {
    if (this != &other) {
        _id = other._id;
        _fromAccountId = other._fromAccountId;
        _toAccountId = other._toAccountId;
        _amount = other._amount;
        _datetime = std::move(other._datetime);

        other._id = 0;
        other._fromAccountId = 0;
        other._toAccountId = 0;
        other._amount = 0;
    }
    return *this;
}

uint64_t TransferEntity::getId() const { return _id; }
uint64_t TransferEntity::getFromAccountId() const { return _fromAccountId; }
uint64_t TransferEntity::getToAccountId() const { return _toAccountId; }
uint64_t TransferEntity::getAmount() const { return _amount; }
const DateTime& TransferEntity::getDateTime() const { return _datetime; }

void TransferEntity::setId(uint64_t id) { _id = id; }
void TransferEntity::setFromAccountId(uint64_t fromAccountId) { _fromAccountId = fromAccountId; }
void TransferEntity::setToAccountId(uint64_t toAccountId) { _toAccountId = toAccountId; }
void TransferEntity::setAmount(uint64_t amount) { _amount = amount; }
void TransferEntity::setDateTime(const DateTime& datetime) { _datetime = datetime; }
