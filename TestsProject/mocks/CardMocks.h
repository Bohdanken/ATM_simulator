#pragma once
#ifndef CARD_MOCKS_H
#define CARD_MOCKS_H

#include <list>
#include <optional>
#include <stdexcept>
#include <cstdint>

struct CardEntity
{
    CardEntity() : _id(0), _accountId(0), _pin(0) {}

    static inline uint64_t freeId = 0; // TODO [T2]: remove after db setup
    /*const*/ uint64_t _id; // TODO [T2]: will be const after db setup
    uint64_t _accountId;
    uint16_t _pin;

    inline uint64_t getId() const { return _id; }
    inline uint64_t getAccountId() const { return _accountId; }
    inline uint16_t getPin() const { return _pin; }

    inline void setAccountId(const uint64_t accountId) { _accountId = accountId; }
    inline void setPin(const uint16_t pin) { _pin = pin; }
};

struct CardDTO
{
    CardDTO() : _id(0), _accountId(0), _pin(0) {}

    CardDTO(const CardEntity &entity) :
        _id(entity.getId()),
        _accountId(entity.getAccountId()),
        _pin(entity.getPin())
    {
    }

    uint64_t _id;
    uint64_t _accountId;
    uint16_t _pin;

    inline uint64_t getId() const { return _id; }
    inline uint64_t getAccountId() const { return _accountId; }
    inline uint16_t getPin() const { return _pin; }

    inline void setId(const uint64_t id) { _id = id; }
    inline void setAccountId(const uint64_t accountId) { _accountId = accountId; }
    inline void setPin(const uint16_t pin) { _pin = pin; }
};

struct CardRepository
{
    // For tests
    static inline void clear() { cards.clear(); }

    static inline std::optional<CardEntity> getById(const uint64_t id)
    {
        for (const CardEntity &entity : cards)
        {
            if (entity.getId() == id)
                return entity;
        }
        return std::nullopt;
    }

    static inline std::list<CardEntity> getAll() { return cards; }

    static inline void save(const CardEntity &entity) { cards.push_back(entity); }

    static inline void update(const uint64_t id, const CardDTO &dto)
    {
        for (CardEntity &entity : cards)
        {
            if (entity.getId() == id)
            {
                entity.setAccountId(dto.getAccountId());
                entity.setPin(dto.getPin());
                break;
            }
        }
    }

    static inline void remove(const uint64_t id)
    {
        cards.remove_if(
            [id](const CardEntity &entity) -> bool { return entity.getId() == id; }
        );
    }

    // TODO: remove after db setup
    static inline std::list<CardEntity> cards;
};

struct CardService
{
    static inline CardDTO create(const CardDTO &dto)
    {
        validateCreate(dto);
        CardEntity entity;
        entity._id = entity.freeId++; // will be handled by the database
        entity.setAccountId(dto.getAccountId());
        entity.setPin(dto.getPin());
        CardRepository::save(entity);
        return CardDTO(entity); // TODO [T2]: add support for move semantics
    }

    static inline void update(const uint64_t id, const CardDTO &dto)
    {
        validateUpdate(id, dto);
        CardRepository::update(id, dto);
    }

    static inline void remove(const uint64_t id)
    {
        const std::optional<CardEntity> &optEntity(CardRepository::getById(id));
        if (!optEntity.has_value())
            throw std::invalid_argument("Error: Cannot delete a card that doesn't exist");
        CardRepository::remove(id);
    }

    static inline CardDTO getById(const uint64_t id)
    {
        const std::optional<CardEntity> &optEntity(CardRepository::getById(id));
        if (!optEntity.has_value())
            throw std::invalid_argument("Error: Cannot get a DTO of a card that does not exist");
        return CardDTO(optEntity.value());
    }

    static inline void validateCreate(const CardDTO &dto)
    {
        const std::optional<AccountEntity> &optAccountEntity(AccountRepository::getById(dto.getAccountId()));
        if (!optAccountEntity.has_value())
            throw std::invalid_argument("Error: Cannot create a card for an account that doesn't exist");

        validatePin(dto);
        for (const CardEntity &entity : CardRepository::getAll())
        {
            if (entity.getAccountId() == dto.getAccountId())
                throw std::invalid_argument("Error: Trying to create a duplicate card for an account");
        }
    }

    static inline void validateUpdate(const uint64_t id, const CardDTO &dto)
    {
        const std::optional<CardEntity> &optEntity(CardRepository::getById(id));
        if (!optEntity.has_value())
            throw std::invalid_argument("Error: Cannot update a card that doesn't exist");

        const std::optional<AccountEntity> &optAccountEntity(AccountRepository::getById(dto.getAccountId()));
        if (!optAccountEntity.has_value())
            throw std::invalid_argument("Error: Cannot update a card to be for an account that doesn't exist");

        validatePin(dto);
        for (const CardEntity &entity : CardRepository::getAll())
        {
            if (entity.getAccountId() == dto.getAccountId() && entity.getId() != id)
                throw std::invalid_argument("Error: Cannot update card to become duplicate for an account");
        }
    }

    static inline void validatePin(const CardDTO &dto)
    {
        if (dto.getPin() > 9999)
            throw std::invalid_argument("Error: Card pin must be in range from 0000 to 9999");
    }
};

#endif // !CARD_MOCKS_H
