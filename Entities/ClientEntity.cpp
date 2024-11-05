// ClientEntity.cpp
#include "ClientEntity.h"

ClientEntity::ClientEntity() : _id(0) {}

ClientEntity::ClientEntity(const ClientEntity& other)
    : _id(other._id), _userName(other._userName), _name(other._name), _email(other._email) {}

ClientEntity& ClientEntity::operator=(const ClientEntity& other) {
    if (this != &other) {
        _id = other._id;
        _userName = other._userName;
        _name = other._name;
        _email = other._email;
    }
    return *this;
}

ClientEntity::ClientEntity(ClientEntity&& other) noexcept
    : _id(other._id),
    _userName(std::move(other._userName)),
    _name(std::move(other._name)),
    _email(std::move(other._email)) {
    other._id = 0;
}

ClientEntity& ClientEntity::operator=(ClientEntity&& other) noexcept {
    if (this != &other) {
        _id = other._id;
        _userName = std::move(other._userName);
        _name = std::move(other._name);
        _email = std::move(other._email);

        other._id = 0;
    }
    return *this;
}

uint64_t ClientEntity::getId() const { return _id; }
const std::string& ClientEntity::getUserName() const { return _userName; }
const std::string& ClientEntity::getName() const { return _name; }
const std::string& ClientEntity::getEmail() const { return _email; }

void ClientEntity::setId(uint64_t id) { _id = id; }
void ClientEntity::setUserName(const std::string& userName) { _userName = userName; }
void ClientEntity::setName(const std::string& name) { _name = name; }
void ClientEntity::setEmail(const std::string& email) { _email = email; }
