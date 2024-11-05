#pragma once
#ifndef TRANSFER_DAEMON_MOCKS_H
#define TRANSFER_DAEMON_MOCKS_H

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <functional>
#include <iostream>
#include <thread>
#include "../DTO/AccountDTO.h"

class TransferDaemon {
public:
    TransferDaemon(boost::asio::io_context& io, AccountDTO* fromAcc, AccountDTO* toAcc,
        std::chrono::seconds freq, double amt, std::function<bool()> cond)
        : from(fromAcc), to(toAcc), timer(io, freq), frequency(freq), amount(amt), condition(cond) {
        scheduleTransfer();
    }


private:
    AccountDTO* from;
    AccountDTO* to;
    boost::asio::steady_timer timer;
    std::chrono::seconds frequency;
    const double amount;
    std::function<bool()> condition;

    void scheduleTransfer() {
        timer.async_wait(
            [this](const boost::system::error_code& ec) {
                if (!ec) {
                    checkAndTransfer();
                    timer.expires_at(timer.expiry() + frequency);
                    scheduleTransfer();
                }
                else {
                    std::cerr << "Error in timer: " << ec.message() << std::endl;
                }
            }
        );
    }

    void checkAndTransfer() const {
        if (condition()) {
            std::cout << "Transfer daemon: works" << std::endl;
            // TODO: define how to use Transaction
        }
    }
};

#endif // !TRANSFER_DAEMON_MOCKS_H
