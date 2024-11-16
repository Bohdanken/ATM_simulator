// CardRepository.cpp
#include "CardRepository.h"

CardRepository::CardRepository(const std::string& connectionStr)
    : conn(connectionStr)
{
    if (!conn.is_open()) {
        throw std::runtime_error("Unable to open the database connection.");
    }

    // Prepare statements once on connection
    conn.prepare("get_card_by_id",
        "SELECT id, account_id, number, pin FROM card WHERE id = $1;");
    conn.prepare("save_card",
        "INSERT INTO card (account_id, number, pin) VALUES ($1, $2, $3) RETURNING id;");
    conn.prepare("update_card",
        "UPDATE card SET number = $1, pin = $2 WHERE id = $3;");
    conn.prepare("remove_card",
        "DELETE FROM card WHERE id = $1;");
}

void CardRepository::clear() {
    try {
        pqxx::work txn(conn);
        txn.exec("DELETE FROM card;");
        txn.commit();
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Failed to clear cards: " + std::string(e.what()));
    }
}

std::optional<CardEntity> CardRepository::getById(uint64_t id) {
    try {
        pqxx::work txn(conn);
        pqxx::result res = txn.exec_prepared("get_card_by_id", id);
        txn.commit();

        if (res.size() == 1) {
            return mapRowToEntity(res[0]);
        }
        return std::nullopt;
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Failed to get card by ID: " + std::string(e.what()));
    }
}

std::list<CardEntity> CardRepository::getAll() {
    std::list<CardEntity> cardsList;
    try {
        pqxx::work txn(conn);
        pqxx::result res = txn.exec("SELECT id, account_id, number, pin FROM card;");
        txn.commit();

        for (const auto& row : res) {
            cardsList.push_back(mapRowToEntity(row));
        }
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Failed to get all cards: " + std::string(e.what()));
    }
    return cardsList;
}

void CardRepository::save(CardEntity& entity) {
    try {
        pqxx::work txn(conn);
        pqxx::result res = txn.exec_prepared("save_card", entity.getAccountId(), entity.getNumber(), entity.getPin());
        txn.commit();

        if (!res.empty()) {
            entity.setId(res[0]["id"].as<uint64_t>()); 
        }
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Failed to save card: " + std::string(e.what()));
    }
}

void CardRepository::update(uint64_t id, const CardDTO& dto) {
    try {
        pqxx::work txn(conn);
        pqxx::result res = txn.exec_prepared("update_card", dto.getNumber(), dto.getPin(), dto.getId());
        txn.commit();

        if (res.affected_rows() == 0) {
            throw std::runtime_error("No card found with the given ID.");
        }
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Failed to update card: " + std::string(e.what()));
    }
}

void CardRepository::remove(uint64_t id) {
    try {
        pqxx::work txn(conn);
        pqxx::result res = txn.exec_prepared("remove_card", id);
        txn.commit();

        if (res.affected_rows() == 0) {
            throw std::runtime_error("No card found with the given ID.");
        }
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Failed to remove card: " + std::string(e.what()));
    }
}

CardEntity CardRepository::mapRowToEntity(const pqxx::row& row) {
    CardEntity entity;
    entity.setId(row["id"].as<uint64_t>());
    entity.setAccountId(row["account_id"].as<uint64_t>());
    entity.setNumber(row["number"].as<int64_t>());
    entity.setPin(row["pin"].as<int>());
    return entity;
}

CardRepository::CardRepository(CardRepository&& other) noexcept
    : conn(std::move(other.conn)) {}

CardRepository& CardRepository::operator=(CardRepository&& other) noexcept {
    if (this != &other) {
        conn = std::move(other.conn);
    }
    return *this;
}
