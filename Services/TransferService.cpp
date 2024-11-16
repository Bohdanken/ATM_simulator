#include "TransferService.h"


void TransferService::setRepository(TransferRepository& transferRepo, AccountRepository& accountRepo) {
    TransferService::transferRepository = &transferRepo;
    TransferService::accountRepository = &accountRepo;
}

TransferDTO TransferService::create(const TransferDTO& dto) {
    validateCreate(dto);
    TransferEntity entity;
    entity.setFromAccountId(dto.getFromAccountId());
    entity.setToAccountId(dto.getToAccountId());
    entity.setAmount(dto.getAmount());
    entity.setDateTime(dto.getDateTime());
    transferRepository->save(entity); 
    return TransferDTO(entity);
}

void TransferService::update(uint64_t id, const TransferDTO& dto) {
    validateUpdate(id, dto);
    transferRepository->update(id, dto); 
}

void TransferService::remove(uint64_t id) {
    auto optEntity = transferRepository->getById(id); 
    if (!optEntity.has_value())
        throw std::invalid_argument("Error: Cannot delete a transfer that doesn't exist");
    transferRepository->remove(id); 
}

TransferDTO TransferService::getById(uint64_t id) {
    auto optEntity = transferRepository->getById(id); 
    if (!optEntity.has_value())
        throw std::invalid_argument("Error: Cannot get a DTO of a transfer that does not exist");
    return TransferDTO(optEntity.value());
}

void TransferService::validateCreate(const TransferDTO& dto) {
    auto optFromAccountEntity = accountRepository->getById(dto.getFromAccountId()); 
    if (!optFromAccountEntity.has_value())
        throw std::invalid_argument("Error: Cannot create a transfer from an account that doesn't exist");

    auto optToAccountEntity = accountRepository->getById(dto.getToAccountId()); 
    if (!optToAccountEntity.has_value())
        throw std::invalid_argument("Error: Cannot create a transfer to an account that doesn't exist");

    validateAccountsAreDifferent(dto);
    validateDatetime(dto);
}

void TransferService::validateUpdate(uint64_t id, const TransferDTO& dto) {
    auto optEntity = transferRepository->getById(id); 
    if (!optEntity.has_value())
        throw std::invalid_argument("Error: Cannot update a transfer that doesn't exist");

    auto optFromAccountEntity = accountRepository->getById(dto.getFromAccountId()); 
    if (!optFromAccountEntity.has_value())
        throw std::invalid_argument("Error: Cannot update a transfer to be from an account that doesn't exist");

    auto optToAccountEntity = accountRepository->getById(dto.getToAccountId()); 
    if (!optToAccountEntity.has_value())
        throw std::invalid_argument("Error: Cannot update a transfer to be to an account that doesn't exist");

    validateAccountsAreDifferent(dto);
    validateDatetime(dto);
}

void TransferService::validateAccountsAreDifferent(const TransferDTO& dto) {
    if (dto.getFromAccountId() == dto.getToAccountId())
        throw std::invalid_argument("Error: Account to transfer funds from cannot be the account to transfer funds to");
}

void TransferService::validateDatetime(const TransferDTO& dto) {
    const DateTime& currentTime = DateTime::getCurrent();
    DateTime cutoffPastTime = currentTime;
    cutoffPastTime.year -= 100;
    if (dto.getDateTime() > currentTime || dto.getDateTime() < cutoffPastTime)
        throw std::invalid_argument("Error: Invalid transfer datetime");
}
