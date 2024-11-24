// ClientEntity.h
#pragma once
#include <string>
#include <cstdint>

/**
 * @brief Represents a Client entity.
 */
class ClientEntity {
public:
    ClientEntity();
    ClientEntity(const ClientEntity& other);
    ClientEntity& operator=(const ClientEntity& other);
    ClientEntity(ClientEntity&& other) noexcept;
    ClientEntity& operator=(ClientEntity&& other) noexcept;

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
