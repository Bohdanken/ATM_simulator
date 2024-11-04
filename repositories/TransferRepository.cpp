#include "TransferRepository.h"
#include <stdexcept>

TransferRepository::TransferRepository(const std::string& connectionStr)
    : conn(connectionStr)
{
    if (!conn.is_open()) {
        throw std::runtime_error("Unable to open the database connection.");
    }

    // Prepare statements once on connection
    conn.prepare("get_transfer_by_id",
        "SELECT id, from_account_id, to_account_id, amount, datetime FROM transfer WHERE id = $1;");
    conn.prepare("save_transfer",
        "INSERT INTO transfer (from_account_id, to_account_id, amount) VALUES ($1, $2, $3) RETURNING id, datetime;");
    conn.prepare("remove_transfer",
        "DELETE FROM transfer WHERE id = $1;");
    conn.prepare("update_transfer",
        "UPDATE transfer SET from_account_id = $1, to_account_id = $2, amount = $3, datetime = $4 WHERE id = $5;");
}

void TransferRepository::clear() {
    try {
        pqxx::work txn(conn);
        txn.exec("DELETE FROM transfer;");
        txn.commit();
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Failed to clear transfers: " + std::string(e.what()));
    }
}

std::optional<TransferEntity> TransferRepository::getById(uint64_t id) {
    try {
        pqxx::work txn(conn);
        pqxx::result res = txn.exec_prepared("get_transfer_by_id", id);
        txn.commit();

        if (res.size() == 1) {
            return mapRowToEntity(res[0]);
        }
        return std::nullopt;
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Failed to get transfer by ID: " + std::string(e.what()));
    }
}

std::list<TransferEntity> TransferRepository::getAll() {
    std::list<TransferEntity> transfersList;
    try {
        pqxx::work txn(conn);
        pqxx::result res = txn.exec("SELECT id, from_account_id, to_account_id, amount, datetime FROM transfer;");
        txn.commit();

        for (const auto& row : res) {
            transfersList.push_back(mapRowToEntity(row));
        }
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Failed to get all transfers: " + std::string(e.what()));
    }
    return transfersList;
}

void TransferRepository::save(TransferEntity& entity) {
    try {
        pqxx::work txn(conn);
        pqxx::result res = txn.exec_prepared("save_transfer", entity.getFromAccountId(), entity.getToAccountId(), entity.getAmount());
        txn.commit();

        if (!res.empty()) {
            entity.setId(res[0]["id"].as<uint64_t>());
            //entity.setDateTime(DateTime::fromString(res[0]["datetime"].c_str())); TODO
        }
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Failed to save transfer: " + std::string(e.what()));
    }
}

void TransferRepository::remove(uint64_t id) {
    try {
        pqxx::work txn(conn);
        pqxx::result res = txn.exec_prepared("remove_transfer", id);
        txn.commit();

        if (res.affected_rows() == 0) {
            throw std::runtime_error("No transfer found with the given ID.");
        }
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Failed to remove transfer: " + std::string(e.what()));
    }
}

void TransferRepository::update(uint64_t id, const TransferDTO& dto) {
    try {
        pqxx::work txn(conn);
        pqxx::result res = txn.exec_prepared("update_transfer",
            dto.getFromAccountId(),
            dto.getToAccountId(),
            dto.getAmount(),
            dto.getDateTime().toString(),  // Convert DateTime to string for SQL
            id
        );
        txn.commit();
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Failed to update transfer: " + std::string(e.what()));
    }
}

TransferEntity TransferRepository::mapRowToEntity(const pqxx::row& row) {
    TransferEntity entity;
    entity.setId(row["id"].as<uint64_t>());
    entity.setFromAccountId(row["from_account_id"].as<uint64_t>());
    entity.setToAccountId(row["to_account_id"].as<uint64_t>());
    entity.setAmount(row["amount"].as<double>());
    //entity.setDateTime(DateTime::fromString(row["datetime"].c_str())); TODO
    return entity;
}
