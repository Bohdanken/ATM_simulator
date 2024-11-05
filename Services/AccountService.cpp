#include "AccountService.h"


AccountRepository AccountService::accountRepository("");



AccountDTO AccountService::create(const AccountDTO& dto) {
    validateCreate(dto);
    AccountEntity entity;
    entity.setNumber(dto.getNumber());
    entity.setBalance(dto.getBalance());
    entity.setClientId(dto.getClientId());
    accountRepository.save(entity);
    return AccountDTO(entity);
}

void AccountService::update(uint64_t id, const AccountDTO& dto) {
    validateUpdate(id, dto);
    accountRepository.update(id, dto);
}

void AccountService::remove(uint64_t id) {
    auto optEntity = accountRepository.getById(id);
    if (!optEntity.has_value())
        throw std::invalid_argument("Error: Cannot delete an account that doesn't exist");
    accountRepository.remove(id);
}

AccountDTO AccountService::getById(uint64_t id) {
    auto optEntity = accountRepository.getById(id);
    if (!optEntity.has_value())
        throw std::invalid_argument("Error: Cannot get a DTO of an account that does not exist");
    return AccountDTO(optEntity.value());
}

void AccountService::deposit(uint64_t id, uint64_t sum) {
    auto optEntity = accountRepository.getById(id);
    if (!optEntity.has_value())
        throw std::invalid_argument("Error: Cannot deposit funds to an account that doesn't exist");
    AccountDTO dto(optEntity.value());
    dto.setBalance(dto.getBalance() + sum);
    accountRepository.update(id, dto);
}

void AccountService::withdraw(uint64_t id, uint64_t sum) {
    auto optEntity = accountRepository.getById(id);
    if (!optEntity.has_value())
        throw std::invalid_argument("Error: Cannot withdraw funds from an account that doesn't exist");
    AccountDTO dto(optEntity.value());
    if (dto.getBalance() < sum)
        throw std::invalid_argument("Error: Trying to withdraw more money than currently available");
    dto.setBalance(dto.getBalance() - sum);
    accountRepository.update(id, dto);
}

void AccountService::validateCreate(const AccountDTO& dto) {
    validateClientId(dto);
    for (const auto& entity : accountRepository.getAll()) {
        if (entity.getNumber() == dto.getNumber())
            throw std::invalid_argument("Error: Account number must be unique");
    }
}

void AccountService::validateUpdate(uint64_t id, const AccountDTO& dto) {
    auto optEntity = accountRepository.getById(id);
    if (!optEntity.has_value())
        throw std::invalid_argument("Error: Cannot update an account that doesn't exist");
    validateClientId(dto);
    for (const auto& entity : accountRepository.getAll()) {
        if (entity.getNumber() == dto.getNumber() && entity.getId() != id)
            throw std::invalid_argument("Error: Cannot update to an account number that is already in use");
    }
}

void AccountService::validateClientId(const AccountDTO& dto) {
    if (dto.getClientId()==-1)
        throw std::invalid_argument("Error: Account ClientId cannot be empty");
}
