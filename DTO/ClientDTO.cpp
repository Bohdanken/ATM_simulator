// ClientDTO.cpp
#include "ClientDTO.h"

ClientDTO::ClientDTO() : _id(0) {}

ClientDTO::ClientDTO(const ClientEntity& entity)
    : _id(entity.getId()),
    _userName(entity.getUserName()),
    _name(entity.getName()),
    _email(entity.getEmail()) {}

ClientDTO::ClientDTO(const ClientDTO& other)
    : _id(other._id), _userName(other._userName), _name(other._name), _email(other._email) {}

ClientDTO::ClientDTO(const std::string& userName, const std::string& name, const std::string& email)
    : _id(0), _userName(userName), _name(name), _email(email) {
    // Поле `_id` ініціалізується значенням за замовчуванням (0)
}

ClientDTO& ClientDTO::operator=(const ClientDTO& other) {
    if (this != &other) {
        _id = other._id;
        _userName = other._userName;
        _name = other._name;
        _email = other._email;
    }
    return *this;
}

ClientDTO::ClientDTO(ClientDTO&& other) noexcept
    : _id(other._id),
    _userName(std::move(other._userName)),
    _name(std::move(other._name)),
    _email(std::move(other._email)) {
    other._id = 0;
}

ClientDTO& ClientDTO::operator=(ClientDTO&& other) noexcept {
    if (this != &other) {
        _id = other._id;
        _userName = std::move(other._userName);
        _name = std::move(other._name);
        _email = std::move(other._email);

        other._id = 0;
    }
    return *this;
}

uint64_t ClientDTO::getId() const { return _id; }
const std::string& ClientDTO::getUserName() const { return _userName; }
const std::string& ClientDTO::getName() const { return _name; }
const std::string& ClientDTO::getEmail() const { return _email; }

void ClientDTO::setId(uint64_t id) { _id = id; }
void ClientDTO::setUserName(const std::string& userName) { _userName = userName; }
void ClientDTO::setName(const std::string& name) { _name = name; }
void ClientDTO::setEmail(const std::string& email) { _email = email; }
