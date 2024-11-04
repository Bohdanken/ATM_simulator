// TransferEntity.h
#pragma once
#include <cstdint>
#include <string>
#include "../utils/time_utils.h"

/**
 * @brief Represents a Transfer entity.
 */
class TransferEntity {
public:
    TransferEntity();
    TransferEntity(const TransferEntity& other) = default;
    TransferEntity& operator=(const TransferEntity& other) = default;
    TransferEntity(TransferEntity&& other) noexcept;
    TransferEntity& operator=(TransferEntity&& other) noexcept;

    uint64_t getId() const;
    uint64_t getFromAccountId() const;
    uint64_t getToAccountId() const;
    uint64_t getAmount() const;
    const DateTime& getDateTime() const;

    void setId(uint64_t id);
    void setFromAccountId(uint64_t fromAccountId);
    void setToAccountId(uint64_t toAccountId);
    void setAmount(uint64_t amount);
    void setDateTime(const DateTime& datetime);

private:
    uint64_t _id;
    uint64_t _fromAccountId;
    uint64_t _toAccountId;
    uint64_t _amount;
    DateTime _datetime;
};
