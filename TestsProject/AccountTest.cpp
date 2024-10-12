#include <string>
#include <vector>
#include <optional>
#include <stdexcept>
#include <cstdint>
#include <cassert>

#pragma region Mock definitions
struct AccountView
{
    uint64_t _id;
    uint64_t _accNumber; // or std::string?
    std::string _userName;

    inline uint64_t getId() const { return _id; }
    inline uint64_t getAccNumber() const { return _accNumber; }
    inline const std::string getUserName() const { return _userName; }

    inline void setId(const uint64_t id) { _id = id; }
    inline void setAccNumber(const uint64_t accNumber) { _accNumber = accNumber; }
    inline void setUserName(const std::string &userName) { _userName = userName; }
};

struct AccountEntity
{
    static inline uint64_t freeId = 0; // TODO: remove after db setup
    /*const*/ uint64_t _id; // will be const after db setup
    uint64_t _accNumber; // or std::string?
    std::string _userName;

    inline uint64_t getId() const { return _id; }
    inline uint64_t getAccNumber() const { return _accNumber; }
    inline const std::string getUserName() const { return _userName; }

    inline void setAccNumber(const uint64_t accNumber) { _accNumber = accNumber; }
    inline void setUserName(const std::string &userName) { _userName = userName; }
};

struct AccountRepository
{
    // Consider: returning empty optional vs throwing an exception
    static inline std::optional<AccountEntity> getById(const uint64_t id)
    {
        for (const AccountEntity &entity : accounts)
        {
            if (entity.getId() == id)
                return entity;
        }
        return std::nullopt;
    }

    static inline void save(const AccountEntity &entity) { accounts.push_back(entity); }

    // TODO: remove after db setup
    static inline std::vector<AccountEntity> accounts;
};

struct AccountService
{
    static inline AccountEntity create(const AccountView &view)
    {
        AccountEntity entity;
        entity._id = entity.freeId++; // will be handled by the database
        entity.setAccNumber(view.getAccNumber());
        entity.setUserName(view.getUserName());
        return entity;
    }

    // Consider: returning empty optional vs throwing an exception
    static inline AccountView getById(const uint64_t id)
    {
        const std::optional<AccountEntity> &opt_entity(AccountRepository::getById(id));
        if (!opt_entity.has_value())
            throw std::out_of_range("Error: No account present with id " + std::to_string(id));

        const AccountEntity &entity(opt_entity.value());
        AccountView view;
        view.setId(entity.getId());
        view.setAccNumber(entity.getAccNumber());
        view.setUserName(entity.getUserName());
        return view;
    }
    // update, delete...
};
#pragma endregion

#pragma region Tests implementation
void testCreateAccount()
{
    AccountView sourceView;
    sourceView.setAccNumber(532);
    sourceView.setUserName("Oleksiy");

    const AccountEntity &entity = AccountService::create(sourceView);
    AccountRepository::save(entity); // Could be moved into AccountService::create
    
    assert(AccountRepository::getById(entity.getId()).has_value());

    const AccountView &resultView(AccountService::getById(entity.getId()));
    assert(resultView.getAccNumber() == sourceView.getAccNumber());
    assert(resultView.getUserName() == sourceView.getUserName());
}
#pragma endregion
