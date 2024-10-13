#include "AccountMocks.h"

#include <list>
#include <optional>
#include <stdexcept>
#include <cstdint>

struct CardEntity
{
    CardEntity() : _id(0), _accountId(0), _pin(0) {}

    static inline uint64_t freeId = 0; // TODO [T2]: remove after db setup
    /*const*/ uint64_t _id; // TODO [T2]: will be const after db setup
    uint64_t _accountId;
    uint16_t _pin;

    inline uint64_t getId() const { return _id; }
    inline uint64_t getAccountId() const { return _accountId; }
    inline uint16_t getPin() const { return _pin; }

    inline void setAccountId(const uint64_t accountId) { _accountId = accountId; }
    inline void setPin(const uint16_t pin) { _pin = pin; }
};

struct CardDTO
{
    CardDTO() : _id(0), _accountId(0), _pin(0) {}

    CardDTO(const CardEntity &entity) :
        _id(entity.getId()), 
        _accountId(entity.getAccountId()),
        _pin(entity.getPin())
    {
    }

    uint64_t _id;
    uint64_t _accountId;
    uint16_t _pin;

    inline uint64_t getId() const { return _id; }
    inline uint64_t getAccountId() const { return _accountId; }
    inline uint16_t getPin() const { return _pin; }

    inline void setId(const uint64_t id) { _id = id; }
    inline void setAccountId(const uint64_t accountId) { _accountId = accountId; }
    inline void setPin(const uint16_t pin) { _pin = pin; }
};

struct CardRepository
{
    // For tests
    static inline void clear() { cards.clear(); }

    static inline std::optional<CardEntity> getById(const uint64_t id)
    {
        for (const CardEntity &entity : cards)
        {
            if (entity.getId() == id)
                return entity;
        }
        return std::nullopt;
    }

    static inline std::list<CardEntity> cards;
};

static inline void test()
{
    AccountDTO sourceDTO;
    sourceDTO.setNumber(532);
    sourceDTO.setBalance(2000);
    sourceDTO.setUserName("Oleksiy");

    const AccountDTO &resultDTO(AccountService::create(sourceDTO));

    CardDTO card;
    card.setAccountId(resultDTO.getId());
    card.setPin(1234);
}