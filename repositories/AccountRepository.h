#ifndef ACCOUNT_REPOSITORY_H
#define ACCOUNT_REPOSITORY_H

#include <optional>
#include <list>
#include <cstdint>
#include <string>
#include <pqxx/pqxx>
#include "../Entities/AccountEntity.h"
#include <stdexcept>
#include <sstream>
#include "../DTO/AccountDTO.h"



class AccountRepository {
public:
    AccountRepository(const std::string& connectionStr);


    ~AccountRepository()=default;


    void clear();


    std::optional<AccountEntity> getById(uint64_t id);


    std::list<AccountEntity> getAll();


    void save( AccountEntity& entity);


    void update(uint64_t id, const AccountDTO& dto);

    void remove(uint64_t id);

    AccountRepository(const AccountRepository&) = delete;
    AccountRepository& operator=(const AccountRepository&) = delete;
    AccountRepository(AccountRepository&& other) noexcept;
    AccountRepository& operator=(AccountRepository&& other) noexcept;


private:
    AccountEntity mapRowToEntity(const pqxx::row& row);

    // libpqxx connection object
     pqxx::connection conn;
};

#endif // ACCOUNT_REPOSITORY_H
