// CardEntity.h
#pragma once
#include <cstdint>
#include <pqxx/pqxx>
#include <optional>
#include <list>


struct CardEntity {
    uint64_t id = 0;        // Automatically assigned ID (BIGSERIAL)
    uint64_t accountId;     // account_id
    int64_t number;         // number
    int pin;                // pin
};

struct CardDTO {
    int64_t number;
    int pin;
};

class CardRepository {
public:
    /**
     * @brief Constructs a CardRepository with the given connection string.
     *
     * @param connectionStr PostgreSQL connection string.
     */
    CardRepository(const std::string& connectionStr);

    /**
     * @brief Destructor to clean up the database connection.
     */
    ~CardRepository() = default;

    /**
     * @brief Clears all cards from the database.
     *
     * @throws std::runtime_error if the operation fails.
     */
    void clear();

    /**
     * @brief Retrieves a card by its ID.
     *
     * @param id The unique identifier of the card.
     * @return std::optional<CardEntity> The card if found, otherwise std::nullopt.
     *
     * @throws std::runtime_error if the operation fails.
     */
    std::optional<CardEntity> getById(uint64_t id);

    /**
     * @brief Retrieves all cards from the database.
     *
     * @return std::list<CardEntity> A list of all cards.
     *
     * @throws std::runtime_error if the operation fails.
     */
    std::list<CardEntity> getAll();

    /**
     * @brief Saves a new card to the database.
     *
     * @param entity The card entity to save.
     *
     * @throws std::runtime_error if the operation fails.
     */
    void save(CardEntity& entity);

    /**
     * @brief Updates an existing card's information.
     *
     * @param id The unique identifier of the card to update.
     * @param dto The data transfer object containing updated information.
     *
     * @throws std::runtime_error if the operation fails or the card is not found.
     */
    void update(uint64_t id, const CardDTO& dto);

    /**
     * @brief Removes a card from the database by its ID.
     *
     * @param id The unique identifier of the card to remove.
     *
     * @throws std::runtime_error if the operation fails.
     */
    void remove(uint64_t id);

private:
    /**
     * @brief Helper method to map a database row to a CardEntity.
     *
     * @param row The database row.
     * @return CardEntity The mapped card entity.
     */
    CardEntity mapRowToEntity(const pqxx::row& row);

    // libpqxx connection object
    pqxx::connection conn;
};
