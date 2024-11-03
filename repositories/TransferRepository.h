// TransferEntity.h
#pragma once
#include <cstdint>
#include <string>
#include <pqxx/pqxx>
#include <optional>
#include <list>
#include "TransferEntity.h"

struct TransferEntity {
    uint64_t id = 0;              // Automatically assigned ID (BIGSERIAL)
    uint64_t fromAccountId;       // from_account_id
    uint64_t toAccountId;         // to_account_id
    double amount;                // amount
    std::string datetime;         // datetime (TIMESTAMP as string)
};

struct TransferDTO {
    uint64_t fromAccountId;
    uint64_t toAccountId;
    double amount;
};

struct TransferRepository {
public:
    /**
     * @brief Constructs a TransferRepository with the given connection string.
     *
     * @param connectionStr PostgreSQL connection string.
     *                      Example: "host=localhost port=5432 dbname=yourdb user=youruser password=yourpassword"
     */
    TransferRepository(const std::string& connectionStr);

    /**
     * @brief Destructor to clean up the database connection.
     */
    ~TransferRepository() = default;

    /**
     * @brief Clears all transfers from the database.
     *
     * @throws std::runtime_error if the operation fails.
     */
    void clear();

    /**
     * @brief Retrieves a transfer by its ID.
     *
     * @param id The unique identifier of the transfer.
     * @return std::optional<TransferEntity> The transfer if found, otherwise std::nullopt.
     *
     * @throws std::runtime_error if the operation fails.
     */
    std::optional<TransferEntity> getById(uint64_t id);

    /**
     * @brief Retrieves all transfers from the database.
     *
     * @return std::list<TransferEntity> A list of all transfers.
     *
     * @throws std::runtime_error if the operation fails.
     */
    std::list<TransferEntity> getAll();

    /**
     * @brief Saves a new transfer to the database.
     *
     * @param entity The transfer entity to save.
     *
     * @throws std::runtime_error if the operation fails.
     */
    void save(TransferEntity& entity);

    /**
     * @brief Removes a transfer from the database by its ID.
     *
     * @param id The unique identifier of the transfer to remove.
     *
     * @throws std::runtime_error if the operation fails.
     */
    void remove(uint64_t id);

private:
    /**
     * @brief Helper method to map a database row to a TransferEntity.
     *
     * @param row The database row.
     * @return TransferEntity The mapped transfer entity.
     */
    TransferEntity mapRowToEntity(const pqxx::row& row);

    // libpqxx connection object
    pqxx::connection conn;
};

