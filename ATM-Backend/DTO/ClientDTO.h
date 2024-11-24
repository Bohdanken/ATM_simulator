#pragma once
#include <string>
#include <cstdint>
#include "../Entities/ClientEntity.h"

/**
 * @brief Data Transfer Object for Client.
 */
class ClientDTO {
public:
    ClientDTO();
    ClientDTO(const std::string& userName, const std::string& name, const std::string& email); // Новий конструктор
    ClientDTO(const ClientEntity& entity);
    ClientDTO(const ClientDTO& other);
    ClientDTO& operator=(const ClientDTO& other);
    ClientDTO(ClientDTO&& other) noexcept;
    ClientDTO& operator=(ClientDTO&& other) noexcept;

    uint64_t getId() const;
    const std::string& getUserName() const;
    const std::string& getName() const;
    const std::string& getEmail() const;

    void setId(uint64_t id);
    void setUserName(const std::string& userName);
    void setName(const std::string& name);
    void setEmail(const std::string& email);

private:
    uint64_t _id;
    std::string _userName;
    std::string _name;
    std::string _email;
};
