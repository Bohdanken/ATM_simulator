#ifndef ACCOUNT_REPOSITORY_H
#define ACCOUNT_REPOSITORY_H

#include <optional>
#include <list>
#include <cstdint>
#include <string>
#include <pqxx/pqxx>

// Forward declarations (Ensure these are properly defined in your project)
struct AccountEntity {
    uint64_t id;
    std::string number;
    double balance;
    std::string userName;

};

struct AccountDTO {
    std::string number;
    double balance;
    std::string userName;

};

class AccountRepository {
public:
    /**
     * @brief Constructs an AccountRepository with the given connection string.
     *
     * @param connectionStr PostgreSQL connection string.
     *                     Example: "host=localhost port=5432 dbname=yourdb user=youruser password=yourpassword"
     */
    AccountRepository(const std::string& connectionStr);

    /**
     * @brief Destructor to clean up the database connection.
     */
    ~AccountRepository()=default;

    /**
     * @brief Clears all accounts from the database.
     *
     * @throws std::runtime_error if the operation fails.
     */
    void clear();

    /**
     * @brief Retrieves an account by its ID.
     *
     * @param id The unique identifier of the account.
     * @return std::optional<AccountEntity> The account if found, otherwise std::nullopt.
     *
     * @throws std::runtime_error if the operation fails.
     */
    std::optional<AccountEntity> getById(uint64_t id);

    /**
     * @brief Retrieves all accounts from the database.
     *
     * @return std::list<AccountEntity> A list of all accounts.
     *
     * @throws std::runtime_error if the operation fails.
     */
    std::list<AccountEntity> getAll();

    /**
     * @brief Saves a new account to the database.
     *
     * @param entity The account entity to save.
     *
     * @throws std::runtime_error if the operation fails.
     */
    void save(const AccountEntity& entity);

    /**
     * @brief Updates an existing account's information.
     *
     * @param id The unique identifier of the account to update.
     * @param dto The data transfer object containing updated information.
     *
     * @throws std::runtime_error if the operation fails or the account is not found.
     */
    void update(uint64_t id, const AccountDTO& dto);

    /**
     * @brief Removes an account from the database by its ID.
     *
     * @param id The unique identifier of the account to remove.
     *
     * @throws std::runtime_error if the operation fails.
     */
    void remove(uint64_t id);

private:
    /**
     * @brief Helper method to map a database row to an AccountEntity.
     *
     * @param row The database row.
     * @return AccountEntity The mapped account entity.
     */
    AccountEntity mapRowToEntity(const pqxx::row& row);

    // libpqxx connection object
    pqxx::connection conn;
};

#endif // ACCOUNT_REPOSITORY_H
