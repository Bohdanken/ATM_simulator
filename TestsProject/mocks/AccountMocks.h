#pragma once
#ifndef ACCOUNT_MOCKS_H
#define ACCOUNT_MOCKS_H

#include <string>
#include <list>
#include <optional>
#include <stdexcept>
#include <cstdint>

struct AccountEntity
{
    AccountEntity() : _id(0), _number(0), _balance(0) {}

    static inline uint64_t freeId = 0; // TODO [T2]: remove after db setup
    /*const*/ uint64_t _id; // TODO [T2]: will be const after db setup
    uint64_t _number;
    uint64_t _balance;
    std::string _userName;

    inline uint64_t getId() const { return _id; }
    inline uint64_t getNumber() const { return _number; }
    inline uint64_t getBalance() const { return _balance; }
    inline const std::string getUserName() const { return _userName; }

    inline void setNumber(const uint64_t number) { _number = number; }
    inline void setBalance(const uint64_t balance) { _balance = balance; }
    inline void setUserName(const std::string &userName) { _userName = userName; }
};

struct AccountDTO
{
    AccountDTO() : _id(0), _number(0), _balance(0) {}

    AccountDTO(const AccountEntity &entity) :
        _id(entity.getId()),
        _number(entity.getNumber()),
        _balance(entity.getBalance()),
        _userName(entity.getUserName())
    {
    }

    uint64_t _id;
    uint64_t _number;
    uint64_t _balance;
    std::string _userName;

    inline uint64_t getId() const { return _id; }
    inline uint64_t getNumber() const { return _number; }
    inline uint64_t getBalance() const { return _balance; }
    inline const std::string getUserName() const { return _userName; }

    inline void setId(const uint64_t id) { _id = id; }
    inline void setNumber(const uint64_t number) { _number = number; }
    inline void setBalance(const uint64_t balance) { _balance = balance; }
    inline void setUserName(const std::string &userName) { _userName = userName; }
};

struct AccountRepository
{
    // For tests
    static inline void clear() { accounts.clear(); }

    static inline std::optional<AccountEntity> getById(const uint64_t id)
    {
        for (const AccountEntity &entity : accounts)
        {
            if (entity.getId() == id)
                return entity;
        }
        return std::nullopt;
    }

    static inline std::list<AccountEntity> getAll() { return accounts; }

    static inline void save(const AccountEntity &entity) { accounts.push_back(entity); }

    static inline void update(const uint64_t id, const AccountDTO &dto)
    {
        for (AccountEntity &entity : accounts)
        {
            if (entity.getId() == id)
            {
                entity.setNumber(dto.getNumber());
                entity.setBalance(dto.getBalance());
                entity.setUserName(dto.getUserName());
                break;
            }
        }
    }

    static inline void remove(const uint64_t id)
    {
        accounts.remove_if(
            [id](const AccountEntity &entity) -> bool { return entity.getId() == id; }
        );
    }

    // TODO: remove after db setup
    static inline std::list<AccountEntity> accounts;
};

struct AccountService
{
    static inline AccountDTO create(const AccountDTO &dto)
    {
        validateCreate(dto);
        AccountEntity entity;
        entity._id = entity.freeId++; // will be handled by the database
        entity.setNumber(dto.getNumber());
        entity.setBalance(dto.getBalance());
        entity.setUserName(dto.getUserName());
        AccountRepository::save(entity);
        return AccountDTO(entity);
    }

    static inline void update(const uint64_t id, const AccountDTO &dto)
    {
        validateUpdate(id, dto);
        AccountRepository::update(id, dto);
    }

    static inline void remove(const uint64_t id)
    {
        const std::optional<AccountEntity> &optEntity(AccountRepository::getById(id));
        if (!optEntity.has_value())
            throw std::invalid_argument("Error: Cannot delete an account that doesn't exist");
        AccountRepository::remove(id);
    }

    static inline AccountDTO getById(const uint64_t id)
    {
        const std::optional<AccountEntity> &optEntity(AccountRepository::getById(id));
        if (!optEntity.has_value())
            throw std::invalid_argument("Error: Cannot get a DTO of an account that does not exist");
        return AccountDTO(optEntity.value());
    }

    static inline void deposit(const uint64_t id, const uint64_t sum)
    {
        const std::optional<AccountEntity> &optEntity(AccountRepository::getById(id));
        if (!optEntity.has_value())
            throw std::invalid_argument("Error: Cannot deposit funds to an account that doesn't exist");
        AccountDTO dto(optEntity.value());
        dto.setBalance(dto.getBalance() + sum);
        AccountRepository::update(id, dto);
    }

    static inline void withdraw(const uint64_t id, const uint64_t sum)
    {
        const std::optional<AccountEntity> &optEntity(AccountRepository::getById(id));
        if (!optEntity.has_value())
            throw std::invalid_argument("Error: Cannot withdraw funds from an account that doesn't exist");

        AccountDTO dto(optEntity.value());
        if (dto.getBalance() < sum)
            throw std::invalid_argument("Error: Trying to withdraw more money than currently available");
        dto.setBalance(dto.getBalance() - sum);
        AccountRepository::update(id, dto);
    }

    static inline void validateCreate(const AccountDTO &dto)
    {
        validateName(dto);
        for (const AccountEntity &entity : AccountRepository::getAll())
        {
            if (entity.getNumber() == dto.getNumber())
                throw std::invalid_argument("Error: Account number must be unique");
        }
    }

    static inline void validateUpdate(const uint64_t id, const AccountDTO &dto)
    {
        const std::optional<AccountEntity> &optEntity(AccountRepository::getById(id));
        if (!optEntity.has_value())
            throw std::invalid_argument("Error: Cannot update an account that doesn't exist");

        validateName(dto);
        for (const AccountEntity &entity : AccountRepository::getAll())
        {
            if (entity.getNumber() == dto.getNumber() && entity.getId() != id)
                throw std::invalid_argument("Error: Cannot update to an account number that is already in use");
        }
    }

    static inline void validateName(const AccountDTO &dto)
    {
        if (dto.getUserName().empty())
            throw std::invalid_argument("Error: Account user name cannot be empty");
    }
};

#endif // !ACCOUNT_MOCKS_H
