#pragma once
#include <cstdint>
#include <string>
#include <pqxx/pqxx>
#include <optional>
#include <list>
#include "../Entities/TransferEntity.h"
#include "../utils/time_utils.h"
#include "../DTO/TransferDTO.h"

struct TransferRepository {
public:

    explicit TransferRepository(const std::string& connectionStr);


    ~TransferRepository() = default;

    TransferRepository(TransferRepository&& other) noexcept;


    TransferRepository& operator=(TransferRepository&& other) noexcept;

 
    void clear();

    std::optional<TransferEntity> getById(uint64_t id);

    std::list<TransferEntity> getAll();

    void save(TransferEntity& entity);

    void remove(uint64_t id);

    void update(uint64_t id, const TransferDTO& dto);

    TransferRepository(const TransferRepository&) = delete;
    TransferRepository& operator=(const TransferRepository&) = delete;

private:

    TransferEntity mapRowToEntity(const pqxx::row& row);

    // libpqxx connection object
    pqxx::connection conn;
};
