// ClientRepository.cpp
#include "ClientRepository.h"
#include <stdexcept>
#include <sstream>

ClientRepository::ClientRepository(const std::string& connectionStr)
    : conn(connectionStr)
{
    if (!conn.is_open()) {
        throw std::runtime_error("Unable to open the database connection.");
    }

    // Prepare statements once on connection
    conn.prepare("get_client_by_id",
        "SELECT id, user_name, name, email FROM client WHERE id = $1;");
    conn.prepare("save_client",
        "INSERT INTO client (user_name, name, email) VALUES ($1, $2, $3) RETURNING id;");
    conn.prepare("update_client",
        "UPDATE client SET user_name = $1, name = $2, email = $3 WHERE id = $4;");
    conn.prepare("remove_client",
        "DELETE FROM client WHERE id = $1;");
}

void ClientRepository::clear() {
    try {
        pqxx::work txn(conn);
        txn.exec("DELETE FROM client;");
        txn.commit();
    }
    catch (const std::exception& e) {
        throw std::runtime_error(std::string("Failed to clear clients: ") + e.what());
    }
}

std::optional<ClientEntity> ClientRepository::getById(uint64_t id) {
    try {
        pqxx::work txn(conn);
        pqxx::result res = txn.exec_prepared("get_client_by_id", id);
        txn.commit();

        if (res.size() == 1) {
            return mapRowToEntity(res[0]);
        }
        return std::nullopt;
    }
    catch (const std::exception& e) {
        throw std::runtime_error(std::string("Failed to get client by ID: ") + e.what());
    }
}

std::list<ClientEntity> ClientRepository::getAll() {
    std::list<ClientEntity> clientsList;
    try {
        pqxx::work txn(conn);
        pqxx::result res = txn.exec("SELECT id, user_name, name, email FROM client;");
        txn.commit();

        for (const auto& row : res) {
            clientsList.push_back(mapRowToEntity(row));
        }
    }
    catch (const std::exception& e) {
        throw std::runtime_error(std::string("Failed to get all clients: ") + e.what());
    }
    return clientsList;
}

void ClientRepository::save(ClientEntity& entity) {
    try {
        pqxx::work txn(conn);
        pqxx::result res = txn.exec_prepared("save_client", entity.userName, entity.name, entity.email);
        txn.commit();

        if (!res.empty()) {
            entity.id = res[0]["id"].as<uint64_t>(); // Update the entity's id with the generated id
        }
    }
    catch (const std::exception& e) {
        throw std::runtime_error(std::string("Failed to save client: ") + e.what());
    }
}

void ClientRepository::update(uint64_t id, const ClientDTO& dto) {
    try {
        pqxx::work txn(conn);
        pqxx::result res = txn.exec_prepared("update_client", dto.userName, dto.name, dto.email, id);
        txn.commit();

        if (res.affected_rows() == 0) {
            throw std::runtime_error("No client found with the given ID.");
        }
    }
    catch (const std::exception& e) {
        throw std::runtime_error(std::string("Failed to update client: ") + e.what());
    }
}

void ClientRepository::remove(uint64_t id) {
    try {
        pqxx::work txn(conn);
        pqxx::result res = txn.exec_prepared("remove_client", id);
        txn.commit();

        if (res.affected_rows() == 0) {
            throw std::runtime_error("No client found with the given ID.");
        }
    }
    catch (const std::exception& e) {
        throw std::runtime_error(std::string("Failed to remove client: ") + e.what());
    }
}

ClientEntity ClientRepository::mapRowToEntity(const pqxx::row& row) {
    ClientEntity entity;
    entity.id = row["id"].as<uint64_t>();
    entity.userName = row["user_name"].as<std::string>();
    entity.name = row["name"].as<std::string>();
    entity.email = row["email"].as<std::string>();
    return entity;
}
