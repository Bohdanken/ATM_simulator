#pragma once
#include <cstdint>
#include <pqxx/pqxx>
#include <optional>
#include <list>
#include "../Entities/CardEntity.h"
#include "../DTO/CardDTO.h"
#include <stdexcept>

struct CardRepository {
public:
 
    explicit CardRepository(const std::string& connectionStr);

 
    ~CardRepository() = default;

  
    CardRepository(CardRepository&& other) noexcept;
    CardRepository& operator=(CardRepository&& other) noexcept;


    void clear();


    std::optional<CardEntity> getById(uint64_t id);


    std::list<CardEntity> getAll();


    void save(CardEntity& entity);


    void update(uint64_t id, const CardDTO& dto);

 
    void remove(uint64_t id);

    CardRepository(const CardRepository&) = delete;
    CardRepository& operator=(const CardRepository&) = delete;

private:

    CardEntity mapRowToEntity(const pqxx::row& row);

    // libpqxx connection object
    pqxx::connection conn;
};
