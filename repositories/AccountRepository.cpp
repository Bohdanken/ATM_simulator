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
    conn.prepare("get_by_id", "SELECT id, user_name FROM client WHERE id = $1;");
    conn.prepare("save_account", "INSERT INTO accounts (id, number, balance, user_name) VALUES ($1, $2, $3, $4);");
    conn.prepare("update_account", "UPDATE accounts SET number = $1, balance = $2, user_name = $3 WHERE id = $4;");
    conn.prepare("remove_account", "DELETE FROM accounts WHERE id = $1;");
}


int test_main(int, char* []) {
    return 0;
}


void AccountRepository::clear() {
    try {
        pqxx::work txn(conn);
        txn.exec("DELETE FROM accounts;");
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
        pqxx::result res = txn.exec("SELECT id, number, balance, user_name FROM accounts;");
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

void AccountRepository::save(const AccountEntity& entity) {
    try {
        pqxx::work txn(conn);
        txn.exec_prepared("save_account", entity.id, entity.number, entity.balance, entity.userName);
        txn.commit();
    }
    catch (const std::exception& e) {
        throw std::runtime_error(std::string("Failed to save account: ") + e.what());
    }
}

void AccountRepository::update(uint64_t id, const AccountDTO& dto) {
    try {
        pqxx::work txn(conn);
        pqxx::result res = txn.exec_prepared("update_account", dto.number, dto.balance, dto.userName, id);
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
    entity.number = row["number"].as<std::string>();
    entity.balance = row["balance"].as<double>();
    entity.userName = row["user_name"].as<std::string>();
    return entity;
}
