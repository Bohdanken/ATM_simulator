#pragma once
#ifndef TRANSFER_DAEMON_H
#define TRANSFER_DAEMON_H

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <functional>
#include <iostream>
#include <thread>
#include "../DTO/AccountDTO.h"
#include "../services/TransferService.h"
#include "../utils/time_utils.h"

class TransferDaemon {
public:
    TransferDaemon(boost::asio::io_context& io, uint64_t fromAccountId, uint64_t toAccountId,
        std::chrono::seconds freq, double amt, std::function<bool()> cond)
        : fromAccountId(fromAccountId), toAccountId(toAccountId), timer(io, freq),
        frequency(freq), amount(amt), condition(cond) {
        scheduleTransfer();
    }

private:
    uint64_t fromAccountId;
    uint64_t toAccountId;
    boost::asio::steady_timer timer;
    std::chrono::seconds frequency;
    double amount;
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

    void checkAndTransfer() {
        if (condition()) {
            try {
                TransferDTO transfer(fromAccountId, toAccountId, amount, DateTime::getCurrent());
                TransferService::create(transfer);
                std::cout << "TransferDaemon: Transferred " << amount
                    << " from Account ID " << fromAccountId
                    << " to Account ID " << toAccountId << std::endl;
            }
            catch (const std::exception& e) {
                std::cerr << "TransferDaemon Error: " << e.what() << std::endl;
            }
        }
    }
};

#endif // TRANSFER_DAEMON_H
