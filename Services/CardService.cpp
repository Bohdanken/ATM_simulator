#include "CardService.h"

//CardService::CardService(CardRepository& cardRepo, AccountRepository& accountRepo)
//    : cardRepository(cardRepo), accountRepository(accountRepo) {}

CardRepository CardService::cardRepository("");
AccountRepository CardService::accountRepository("");


CardDTO CardService::create(const CardDTO& dto) {
    validateCreate(dto);
    CardEntity entity;
    entity.setAccountId(dto.getAccountId());
    entity.setPin(dto.getPin());
    cardRepository.save(entity); 
    return CardDTO(entity);
}

void CardService::update(uint64_t id, const CardDTO& dto) {
    validateUpdate(id, dto);
    cardRepository.update(id, dto);  
}

void CardService::remove(uint64_t id) {
    auto optEntity = cardRepository.getById(id);  // Replaced CardRepository:: with cardRepository
    if (!optEntity.has_value())
        throw std::invalid_argument("Error: Cannot delete a card that doesn't exist");
    cardRepository.remove(id);  // Replaced CardRepository:: with cardRepository
}

CardDTO CardService::getById(uint64_t id) {
    auto optEntity = cardRepository.getById(id);  // Replaced CardRepository:: with cardRepository
    if (!optEntity.has_value())
        throw std::invalid_argument("Error: Cannot get a DTO of a card that does not exist");
    return CardDTO(optEntity.value());
}

void CardService::validateCreate(const CardDTO& dto) {
    auto optAccountEntity = accountRepository.getById(dto.getAccountId());  // Assuming accountRepository is accessible
    if (!optAccountEntity.has_value())
        throw std::invalid_argument("Error: Cannot create a card for an account that doesn't exist");

    validatePin(dto);
    for (const auto& entity : cardRepository.getAll()) {  // Replaced CardRepository:: with cardRepository
        if (entity.getAccountId() == dto.getAccountId())
            throw std::invalid_argument("Error: Trying to create a duplicate card for an account");
    }
}

void CardService::validateUpdate(uint64_t id, const CardDTO& dto) {
    auto optEntity = cardRepository.getById(id);  // Replaced CardRepository:: with cardRepository
    if (!optEntity.has_value())
        throw std::invalid_argument("Error: Cannot update a card that doesn't exist");

    auto optAccountEntity = accountRepository.getById(dto.getAccountId());  // Assuming accountRepository is accessible
    if (!optAccountEntity.has_value())
        throw std::invalid_argument("Error: Cannot update a card to be for an account that doesn't exist");

    validatePin(dto);
    for (const auto& entity : cardRepository.getAll()) {  // Replaced CardRepository:: with cardRepository
        if (entity.getAccountId() == dto.getAccountId() && entity.getId() != id)
            throw std::invalid_argument("Error: Cannot update card to become duplicate for an account");
    }
}

void CardService::validatePin(const CardDTO& dto) {
    if (dto.getPin() > 9999)
        throw std::invalid_argument("Error: Card PIN must be in range from 0000 to 9999");
}
