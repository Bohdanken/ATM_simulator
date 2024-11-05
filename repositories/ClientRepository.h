// ClientRepository.h
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
    /**
     * @brief Constructs a ClientRepository with the given connection string.
     *
     * @param connectionStr PostgreSQL connection string.
     *                     Example: "host=localhost port=5432 dbname=yourdb user=youruser password=yourpassword"
     */
    ClientRepository(const std::string& connectionStr);

    /**
     * @brief Destructor to clean up the database connection.
     */
    ~ClientRepository() = default;

    /**
     * @brief Clears all clients from the database.
     *
     * @throws std::runtime_error if the operation fails.
     */
    void clear();

    /**
     * @brief Retrieves a client by its ID.
     *
     * @param id The unique identifier of the client.
     * @return std::optional<ClientEntity> The client if found, otherwise std::nullopt.
     *
     * @throws std::runtime_error if the operation fails.
     */
    std::optional<ClientEntity> getById(uint64_t id);

    /**
     * @brief Retrieves all clients from the database.
     *
     * @return std::list<ClientEntity> A list of all clients.
     *
     * @throws std::runtime_error if the operation fails.
     */
    std::list<ClientEntity> getAll();

    /**
     * @brief Saves a new client to the database.
     *
     * @param entity The client entity to save.
     *
     * @throws std::runtime_error if the operation fails.
     */
    void save(ClientEntity& entity);

    /**
     * @brief Updates an existing client's information.
     *
     * @param id The unique identifier of the client to update.
     * @param dto The data transfer object containing updated information.
     *
     * @throws std::runtime_error if the operation fails or the client is not found.
     */
    void update(uint64_t id, const ClientDTO& dto);

    /**
     * @brief Removes a client from the database by its ID.
     *
     * @param id The unique identifier of the client to remove.
     *
     * @throws std::runtime_error if the operation fails.
     */
    void remove(uint64_t id);

private:
    /**
     * @brief Helper method to map a database row to a ClientEntity.
     *
     * @param row The database row.
     * @return ClientEntity The mapped client entity.
     */
    ClientEntity mapRowToEntity(const pqxx::row& row);

    // libpqxx connection object
    pqxx::connection conn;
};
