// TODO [T2?->Release]: Replace mocks with final implementations

#include "utils/assert_utils.h"

#pragma region Utils implementation
void accountTestWrapper(void (*const fn)())
{
    AccountRepository::clear();
    fn();
}
#pragma endregion


struct TransferDaemon {
    AccountDTO* from;
    AccountDTO* to;
    boost::asio::steady_timer timer;
    std::chrono::seconds frequency;
    const double amount;
    std::function<bool()> condition;

    TransferDaemon(boost::asio::io_context& io, AccountDTO* fromAcc, AccountDTO* toAcc, std::chrono::seconds freq, double amt, std::function<bool()> cond)
        : from(fromAcc), to(toAcc), timer(io, freq), frequency(freq), amount(amt), condition(cond) {
        scheduleTransfer();
    }


    void scheduleTransfer() {
        timer.async_wait([this](const boost::system::error_code& ec) {
            if (!ec) {
                checkAndTransfer();
                timer.expires_at(timer.expiry() + frequency);
                scheduleTransfer();
            }
            else {
                std::cerr << "Error in timer: " << ec.message() << std::endl;
            }
            });
    }


    void checkAndTransfer() {
        if (condition()) {
            cout << "Yeah, code" << endl;
            //TODO define how to use Transaction
        }
    }
};

