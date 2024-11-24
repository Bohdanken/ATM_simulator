// TransferDTO.cpp
#include "TransferDTO.h"


TransferDTO::TransferDTO()
    : _id(0), _fromAccountId(0), _toAccountId(0), _amount(0), _datetime(DateTime::getCurrent()) {}

TransferDTO::TransferDTO(const TransferEntity& entity)
    : _id(entity.getId()),
    _fromAccountId(entity.getFromAccountId()),
    _toAccountId(entity.getToAccountId()),
    _amount(entity.getAmount()),
    _datetime(entity.getDateTime()) {}

TransferDTO::TransferDTO(TransferDTO&& other) noexcept
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

TransferDTO::TransferDTO(uint64_t fromAccountId, uint64_t toAccountId, uint64_t amount, const DateTime& datetime)
    : _id(0), _fromAccountId(fromAccountId), _toAccountId(toAccountId), _amount(amount), _datetime(datetime) {}

TransferDTO& TransferDTO::operator=(TransferDTO&& other) noexcept {
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

uint64_t TransferDTO::getId() const { return _id; }
uint64_t TransferDTO::getFromAccountId() const { return _fromAccountId; }
uint64_t TransferDTO::getToAccountId() const { return _toAccountId; }
uint64_t TransferDTO::getAmount() const { return _amount; }
const DateTime& TransferDTO::getDateTime() const { return _datetime; }

void TransferDTO::setId(uint64_t id) { _id = id; }
void TransferDTO::setFromAccountId(uint64_t fromAccountId) { _fromAccountId = fromAccountId; }
void TransferDTO::setToAccountId(uint64_t toAccountId) { _toAccountId = toAccountId; }
void TransferDTO::setAmount(uint64_t amount) { _amount = amount; }
void TransferDTO::setDateTime(const DateTime& datetime) { _datetime = datetime; }
