#pragma once
#ifndef TRANSFER_DAEMON_TEST_H
#define TRANSFER_DAEMON_TEST_H

#include "mocks/TransferDaemonMocks.h"

inline void testTransferDaemon()
{
    boost::asio::io_context io;

    AccountDTO fromAccount = AccountDTO();
    AccountDTO toAccount = AccountDTO();
    fromAccount.setUserName("From");
    fromAccount.setBalance(500);
    toAccount.setUserName("To");
    toAccount.setBalance(100);

    auto transferCondition = [&fromAccount]() -> bool { return fromAccount.getBalance() > 200.0; };

    // Create a TransferDaemon that transfers 50 units every second
    TransferDaemon daemon(io, &fromAccount, &toAccount, std::chrono::seconds(1), 50.0, transferCondition);
    io.run();
    std::this_thread::sleep_for(std::chrono::seconds(15));
    assert(toAccount.getBalance() == 600);
}

#endif // !TRANSFER_DAEMON_TEST_H
