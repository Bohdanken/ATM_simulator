#include <cstdint>

struct CardEntity
{
    static inline uint64_t freeId = 0; // TODO [T2]: remove after db setup
    /*const*/ uint64_t _id; // TODO [T2]: will be const after db setup
    uint8_t _pin;

    inline uint64_t getId() const { return _id; }
    inline uint8_t getPin() const { return _pin; }

    inline void setPin(const uint8_t pin) { _pin = pin; }
};