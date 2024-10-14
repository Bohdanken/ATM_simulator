#pragma once
#ifndef TRANSFER_MOCKS_H
#define TRANSFER_MOCKS_H

#include "AccountMocks.h"
#include "../utils/time_utils.h"

#include <list>
#include <optional>
#include <stdexcept>
#include <cstdint>

struct TransferEntity
{
    TransferEntity() : 
        _id(0), 
        _fromAccountId(0), 
        _toAccountId(0), 
        _amount(0), 
        _datetime(DateTime::getCurrent()) 
    {
    }

    static inline uint64_t freeId = 0; // TODO [T2]: remove after db setup
    /*const*/ uint64_t _id; // TODO [T2]: will be const after db setup
    uint64_t _fromAccountId;
    uint64_t _toAccountId;
    uint64_t _amount;
    DateTime _datetime;

    inline uint64_t getId() const { return _id; }
    inline uint64_t getFromAccountId() const { return _fromAccountId; }
    inline uint64_t getToAccountId() const { return _toAccountId; }
    inline uint64_t getAmount() const { return _amount; }
    inline const DateTime getDateTime() const { return _datetime; }

    inline void setFromAccountId(const uint64_t fromAccountId) { _fromAccountId = fromAccountId; }
    inline void setToAccountId(const uint64_t toAccountId) { _toAccountId = toAccountId; }
    inline void setAmount(const uint64_t amount) { _amount = amount; }
    inline void setDateTime(const DateTime &datetime) { _datetime = datetime; }
};

struct TransferDTO
{
    TransferDTO() :
        _id(0),
        _fromAccountId(0),
        _toAccountId(0),
        _amount(0),
        _datetime(DateTime::getCurrent())
    {
    }

    TransferDTO(const TransferEntity &entity) :
        _id(entity.getId()),
        _fromAccountId(entity.getFromAccountId()),
        _toAccountId(entity.getToAccountId()),
        _amount(entity.getAmount()),
        _datetime(entity.getDateTime())
    {
    }

    uint64_t _id;
    uint64_t _fromAccountId;
    uint64_t _toAccountId;
    uint64_t _amount;
    DateTime _datetime;

    inline uint64_t getId() const { return _id; }
    inline uint64_t getFromAccountId() const { return _fromAccountId; }
    inline uint64_t getToAccountId() const { return _toAccountId; }
    inline uint64_t getAmount() const { return _amount; }
    inline const DateTime getDateTime() const { return _datetime; }

    inline void setId(const uint64_t id) { _id = id; }
    inline void setFromAccountId(const uint64_t fromAccountId) { _fromAccountId = fromAccountId; }
    inline void setToAccountId(const uint64_t toAccountId) { _toAccountId = toAccountId; }
    inline void setAmount(const uint64_t amount) { _amount = amount; }
    inline void setDateTime(const DateTime &datetime) { _datetime = datetime; }
};

struct TransferRepository
{
    // For tests
    static inline void clear() { transfers.clear(); }

    static inline std::optional<TransferEntity> getById(const uint64_t id)
    {
        for (const TransferEntity &entity : transfers)
        {
            if (entity.getId() == id)
                return entity;
        }
        return std::nullopt;
    }

    static inline std::list<TransferEntity> getAll() { return transfers; }

    static inline void save(const TransferEntity &entity) { transfers.push_back(entity); }

    static inline void update(const uint64_t id, const TransferDTO &dto)
    {
        for (TransferEntity &entity : transfers)
        {
            if (entity.getId() == id)
            {
                entity.setFromAccountId(dto.getFromAccountId());
                entity.setToAccountId(dto.getToAccountId());
                entity.setAmount(dto.getAmount());
                entity.setDateTime(dto.getDateTime());
                break;
            }
        }
    }

    static inline void remove(const uint64_t id)
    {
        transfers.remove_if(
            [id](const TransferEntity &entity) -> bool { return entity.getId() == id; }
        );
    }

    // TODO: remove after db setup
    static inline std::list<TransferEntity> transfers;
};

struct TransferService
{
    static inline TransferDTO create(const TransferDTO &dto)
    {
        validateCreate(dto);
        TransferEntity entity;
        entity._id = entity.freeId++; // will be handled by the database
        entity.setFromAccountId(dto.getFromAccountId());
        entity.setToAccountId(dto.getToAccountId());
        entity.setAmount(dto.getAmount());
        entity.setDateTime(dto.getDateTime());
        TransferRepository::save(entity);
        return TransferDTO(entity); // TODO [T2]: add support for move semantics
    }

    static inline void update(const uint64_t id, const TransferDTO &dto)
    {
        validateUpdate(id, dto);
        TransferRepository::update(id, dto);
    }

    static inline void remove(const uint64_t id)
    {
        const std::optional<TransferEntity> &optEntity(TransferRepository::getById(id));
        if (!optEntity.has_value())
            throw std::invalid_argument("Error: Cannot delete a transfer that doesn't exist");
        TransferRepository::remove(id);
    }

    static inline TransferDTO getById(const uint64_t id)
    {
        const std::optional<TransferEntity> &optEntity(TransferRepository::getById(id));
        if (!optEntity.has_value())
            throw std::invalid_argument("Error: Cannot get a DTO of a transfer that does not exist");
        return TransferDTO(optEntity.value());
    }

    static inline void validateCreate(const TransferDTO &dto)
    {
        const std::optional<AccountEntity> &optFromAccountEntity(AccountRepository::getById(dto.getFromAccountId()));
        if (!optFromAccountEntity.has_value())
            throw std::invalid_argument("Error: Cannot create a transfer from an account that doesn't exist");

        const std::optional<AccountEntity> &optToAccountEntity(AccountRepository::getById(dto.getToAccountId()));
        if (!optToAccountEntity.has_value())
            throw std::invalid_argument("Error: Cannot create a transfer to an account that doesn't exist");

        validateAccountsAreDifferent(dto);
        validateDatetime(dto);
    }

    static inline void validateUpdate(const uint64_t id, const TransferDTO &dto)
    {
        const std::optional<TransferEntity> &optEntity(TransferRepository::getById(id));
        if (!optEntity.has_value())
            throw std::invalid_argument("Error: Cannot update a transfer that doesn't exist");

        const std::optional<AccountEntity> &optFromAccountEntity(AccountRepository::getById(dto.getFromAccountId()));
        if (!optFromAccountEntity.has_value())
            throw std::invalid_argument("Error: Cannot update a transfer to be from an account that doesn't exist");

        const std::optional<AccountEntity> &optToAccountEntity(AccountRepository::getById(dto.getToAccountId()));
        if (!optToAccountEntity.has_value())
            throw std::invalid_argument("Error: Cannot update a transfer to be to an account that doesn't exist");

        validateAccountsAreDifferent(dto);
        validateDatetime(dto);
    }

    static inline void validateAccountsAreDifferent(const TransferDTO &dto)
    {
        if (dto.getFromAccountId() == dto.getToAccountId())
            throw std::invalid_argument("Error: Account to transfer funds from cannot be the account to transfer funds to");
    }

    static inline void validateDatetime(const TransferDTO &dto)
    {
        const DateTime &currentTime(DateTime::getCurrent());
        DateTime cutoffPastTime(currentTime);
        cutoffPastTime.year -= 100;
        if (dto.getDateTime() > currentTime || dto.getDateTime() < cutoffPastTime)
            throw std::invalid_argument("Error: Invalid trasfer datetime");
    }
};

#endif // !TRANSFER_MOCKS_H
