#pragma once
#ifndef TRANSFER_DAEMON_H
#define TRANSFER_DAEMON_H

#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <functional>
#include "../DTO/AccountDTO.h"
#include "../services/TransferService.h"
#include "../utils/time_utils.h"

class TransferDaemon {
public:
    TransferDaemon(uint64_t fromAccountId, uint64_t toAccountId,
        std::chrono::seconds freq, double amt, std::function<bool()> cond)
        : fromAccountId(fromAccountId), toAccountId(toAccountId), frequency(freq),
        amount(amt), condition(cond), isRunning(true) {
        std::cout << "TransferDaemon created for transfer from Account " << fromAccountId
            << " to Account " << toAccountId << std::endl;
        workerThread = std::thread(&TransferDaemon::run, this); // Start the daemon thread
    }

    ~TransferDaemon() {
        stop(); // Ensure thread stops cleanly
    }

    // Stop the daemon
    void stop() {
        isRunning = false;
        if (workerThread.joinable()) {
            workerThread.join();
        }
        std::cout << "TransferDaemon for Account " << fromAccountId
            << " stopped." << std::endl;
    }

private:
    uint64_t fromAccountId;
    uint64_t toAccountId;
    std::chrono::seconds frequency;
    double amount;
    std::function<bool()> condition;
    std::atomic<bool> isRunning; // Thread-safe flag to stop the loop
    std::thread workerThread;    // Daemon thread

    // Thread loop
    void run() {
        while (isRunning) {
            std::this_thread::sleep_for(frequency); // Wait for the frequency duration
            if (isRunning && condition()) { // Check the condition
                try {
                    std::cout << "Daemon triggered for transfer from " << fromAccountId
                        << " to " << toAccountId << " at "
                        << DateTime::getCurrent().toString() << "\n";

                    TransferDTO transfer(fromAccountId, toAccountId, amount, DateTime::getCurrent());
                    TransferService::create(transfer);

                    std::cout << "Transferred " << amount << " from Account " << fromAccountId
                        << " to Account " << toAccountId << std::endl;
                }
                catch (const std::exception& e) {
                    std::cerr << "TransferDaemon Error: " << e.what() << std::endl;
                }
            }
        }
    }
};

#endif // TRANSFER_DAEMON_H
