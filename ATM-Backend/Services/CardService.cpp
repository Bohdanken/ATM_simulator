#include "CardService.h"

void CardService::setRepository(CardRepository& cardRepo, AccountRepository& accountRepo) {
    CardService::accountRepository = &accountRepo;
    CardService::cardRepository = &cardRepo;
}

CardDTO CardService::create(const CardDTO& dto) {
    validateCreate(dto);
    CardEntity entity;
    entity.setNumber(dto.getNumber());
    entity.setAccountId(dto.getAccountId());
    entity.setPin(dto.getPin());
    cardRepository->save(entity);
    return CardDTO(entity);
}

void CardService::update(uint64_t id, const CardDTO& dto) {
    validateUpdate(id, dto);
    cardRepository->update(id, dto);
}

void CardService::remove(uint64_t id) {
    const std::optional<CardEntity>&optEntity = cardRepository->getById(id);
    if (!optEntity.has_value())
        throw std::invalid_argument("Error: Cannot delete a card that doesn't exist");
    cardRepository->remove(id);
}

CardDTO CardService::getById(uint64_t id) {
    auto optEntity = cardRepository->getById(id);
    if (!optEntity.has_value())
        throw std::invalid_argument("Error: Cannot get a DTO of a card that does not exist");
    return CardDTO(optEntity.value());
}

void CardService::validateCreate(const CardDTO& dto) {
    auto optAccountEntity = accountRepository->getById(dto.getAccountId());
    if (!optAccountEntity.has_value())
        throw std::invalid_argument("Error: Cannot create a card for an account that doesn't exist");

    validatePin(dto);
    for (const auto& entity : cardRepository->getAll()) {
        if (entity.getAccountId() == dto.getAccountId())
            throw std::invalid_argument("Error: Trying to create a duplicate card for an account");
    }
}

void CardService::validateUpdate(uint64_t id, const CardDTO& dto) {
    auto optEntity = cardRepository->getById(id);
    if (!optEntity.has_value())
        throw std::invalid_argument("Error: Cannot update a card that doesn't exist");

    auto optAccountEntity = accountRepository->getById(dto.getAccountId());
    if (!optAccountEntity.has_value())
        throw std::invalid_argument("Error: Cannot update a card to be for an account that doesn't exist");

    validatePin(dto);
    for (const auto& entity : cardRepository->getAll()) {
        if (entity.getAccountId() == dto.getAccountId() && entity.getId() != id)
            throw std::invalid_argument("Error: Cannot update card to become duplicate for an account");
    }
}

void CardService::validatePin(const CardDTO& dto) {
    if (dto.getPin() > 9999)
        throw std::invalid_argument("Error: Card PIN must be in range from 0000 to 9999");
}

CardDTO CardService::getByNumber(long long number) {
    auto optEntity = cardRepository->getByNumber(number);
    if (!optEntity.has_value())
        throw std::invalid_argument("Error: Card with the specified number does not exist");
    return CardDTO(optEntity.value());
}

