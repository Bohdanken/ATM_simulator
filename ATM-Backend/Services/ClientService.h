// ClientService.h
#pragma once
#include <stdexcept>
#include <optional>
#include "../DTO/ClientDTO.h"
#include "../repositories/ClientRepository.h"
/**
 * @brief Service class for handling Client operations.
 */
class ClientService {
public:
    static void setRepository(ClientRepository& clientRepository);
    static ClientDTO create(const ClientDTO& dto);
    static void update(uint64_t id, const ClientDTO& dto);
    static void remove(uint64_t id);
    static ClientDTO getById(uint64_t id);

private:
    inline static ClientRepository* clientRepository= nullptr;
    static void validateCreate(const ClientDTO& dto);
    static void validateUpdate(uint64_t id, const ClientDTO& dto);
    static void validateEmail(const ClientDTO& dto);
};
