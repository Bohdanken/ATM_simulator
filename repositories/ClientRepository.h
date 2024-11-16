#pragma once
#include <pqxx/pqxx>
#include <optional>
#include <list>
#include <string>
#include <stdexcept>
#include <sstream>
#include "../Entities/ClientEntity.h"
#include "../DTO/ClientDTO.h"

class ClientRepository {
public:

    explicit ClientRepository(const std::string& connectionStr);

    ~ClientRepository() = default;


    ClientRepository(ClientRepository&& other) noexcept;


    ClientRepository& operator=(ClientRepository&& other) noexcept;


    void clear();


    std::optional<ClientEntity> getById(uint64_t id);


    std::list<ClientEntity> getAll();


    void save(ClientEntity& entity);


    void update(uint64_t id, const ClientDTO& dto);

    void remove(uint64_t id);

    ClientRepository(const ClientRepository&) = delete;
    ClientRepository& operator=(const ClientRepository&) = delete;

private:

    ClientEntity mapRowToEntity(const pqxx::row& row);

    // libpqxx connection object
    pqxx::connection conn;
};
