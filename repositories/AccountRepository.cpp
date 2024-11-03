#include "AccountRepository.h"
#include <pqxx/pqxx>
#include <stdexcept>
#include <sstream>

AccountRepository::AccountRepository(const std::string& connectionStr)
    : conn(connectionStr)
{
    if (!conn.is_open()) {
        throw std::runtime_error("Unable to open the database connection.");
    }

    // Prepare statements once on connection
    conn.prepare("get_by_id", "SELECT id, client_id, number, balance FROM account WHERE id = $1;");
    conn.prepare("save_account", "INSERT INTO account (client_id, number, balance) VALUES ($1, $2, $3) RETURNING id;");
    conn.prepare("update_account", "UPDATE account SET number = $1, balance = $2 WHERE id = $3;");
    conn.prepare("remove_account", "DELETE FROM account WHERE id = $1;");
}

void AccountRepository::clear() {
    try {
        pqxx::work txn(conn);
        txn.exec("DELETE FROM account;");
        txn.commit();
    }
    catch (const std::exception& e) {
        throw std::runtime_error(std::string("Failed to clear accounts: ") + e.what());
    }
}

std::optional<AccountEntity> AccountRepository::getById(uint64_t id) {
    try {
        pqxx::work txn(conn);
        pqxx::result res = txn.exec_prepared("get_by_id", id);
        txn.commit();

        if (res.size() == 1) {
            return mapRowToEntity(res[0]);
        }
        return std::nullopt;
    }
    catch (const std::exception& e) {
        throw std::runtime_error(std::string("Failed to get account by ID: ") + e.what());
    }
}

std::list<AccountEntity> AccountRepository::getAll() {
    std::list<AccountEntity> accountsList;
    try {
        pqxx::work txn(conn);
        pqxx::result res = txn.exec("SELECT id, client_id, number, balance FROM account;");
        txn.commit();

        for (const auto& row : res) {
            accountsList.push_back(mapRowToEntity(row));
        }
    }
    catch (const std::exception& e) {
        throw std::runtime_error(std::string("Failed to get all accounts: ") + e.what());
    }
    return accountsList;
}

void AccountRepository::save(AccountEntity& entity) {
    try {
        pqxx::work txn(conn);
        pqxx::result res = txn.exec_prepared("save_account", entity.clientId, entity.number, entity.balance);
        txn.commit();
    if (!res.empty()) {
        entity.id = res[0]["id"].as<uint64_t>(); // Update the entity's id with the generated id
    }
}catch (const std::exception& e) {
        throw std::runtime_error(std::string("Failed to save account: ") + e.what());
    }
}

void AccountRepository::update(uint64_t id, const AccountDTO& dto) {
    try {
        pqxx::work txn(conn);
        pqxx::result res = txn.exec_prepared("update_account", dto.number, dto.balance, id);
        txn.commit();

        if (res.affected_rows() == 0) {
            throw std::runtime_error("No account found with the given ID.");
        }
    }
    catch (const std::exception& e) {
        throw std::runtime_error(std::string("Failed to update account: ") + e.what());
    }
}

void AccountRepository::remove(uint64_t id) {
    try {
        pqxx::work txn(conn);
        pqxx::result res = txn.exec_prepared("remove_account", id);
        txn.commit();

        if (res.affected_rows() == 0) {
            throw std::runtime_error("No account found with the given ID.");
        }
    }
    catch (const std::exception& e) {
        throw std::runtime_error(std::string("Failed to remove account: ") + e.what());
    }
}

AccountEntity AccountRepository::mapRowToEntity(const pqxx::row& row) {
    AccountEntity entity;
    entity.id = row["id"].as<uint64_t>();
    entity.clientId = row["client_id"].as<uint64_t>();
    entity.number = row["number"].as<int64_t>();
    entity.balance = row["balance"].as<double>();
    return entity;
}
