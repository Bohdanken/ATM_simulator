// TransferDTO.h
#pragma once
#include <cstdint>
#include "../Entities/TransferEntity.h"
#include "../utils/time_utils.h"

/**
 * @brief Data Transfer Object for Transfer.
 */
class TransferDTO {
public:
    TransferDTO();
    TransferDTO(const TransferEntity& entity);
    TransferDTO(const TransferDTO& other) = default;
    TransferDTO& operator=(const TransferDTO& other) = default;
    TransferDTO(TransferDTO&& other) noexcept;
    TransferDTO& operator=(TransferDTO&& other) noexcept;

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
    uint64_t _id;
    uint64_t _fromAccountId;
    uint64_t _toAccountId;
    uint64_t _amount;
    DateTime _datetime;
};
