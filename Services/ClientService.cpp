#include "ClientService.h"


void ClientService::setRepository(ClientRepository& clientRepo) {
    ClientService::clientRepository = &clientRepo;
}

ClientDTO ClientService::create(const ClientDTO& dto) {
    validateCreate(dto);
    ClientEntity entity;
    entity.setUserName(dto.getUserName());
    entity.setName(dto.getName());
    entity.setEmail(dto.getEmail());
    clientRepository->save(entity); // Use -> for pointer access
    return ClientDTO(entity);
}

void ClientService::update(uint64_t id, const ClientDTO& dto) {
    validateUpdate(id, dto);
    clientRepository->update(id, dto); // Use ->
}

void ClientService::remove(uint64_t id) {
    auto optEntity = clientRepository->getById(id); // Use ->
    if (!optEntity.has_value())
        throw std::invalid_argument("Error: Cannot delete a client that doesn't exist");
    clientRepository->remove(id); // Use ->
}

ClientDTO ClientService::getById(uint64_t id) {
    auto optEntity = clientRepository->getById(id); // Use ->
    if (!optEntity.has_value())
        throw std::invalid_argument("Error: Cannot get a DTO of a client that does not exist");
    return ClientDTO(optEntity.value());
}

void ClientService::validateCreate(const ClientDTO& dto) {
    validateEmail(dto);
    for (const auto& entity : clientRepository->getAll()) { // Use ->
        if (entity.getUserName() == dto.getUserName())
            throw std::invalid_argument("Error: Username must be unique");
        if (entity.getEmail() == dto.getEmail())
            throw std::invalid_argument("Error: Email must be unique");
    }
}

void ClientService::validateUpdate(uint64_t id, const ClientDTO& dto) {
    auto optEntity = clientRepository->getById(id); // Use ->
    if (!optEntity.has_value())
        throw std::invalid_argument("Error: Cannot update a client that doesn't exist");
    validateEmail(dto);
    for (const auto& entity : clientRepository->getAll()) { // Use ->
        if (entity.getUserName() == dto.getUserName() && entity.getId() != id)
            throw std::invalid_argument("Error: Cannot update to a username that is already in use");
        if (entity.getEmail() == dto.getEmail() && entity.getId() != id)
            throw std::invalid_argument("Error: Cannot update to an email that is already in use");
    }
}

void ClientService::validateEmail(const ClientDTO& dto) {
    if (dto.getEmail().empty())
        throw std::invalid_argument("Error: Email cannot be empty");
}
