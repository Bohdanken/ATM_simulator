#pragma once
#ifndef TRANSFER_TEST_H
#define TRANSFER_TEST_H

#pragma region Utils
void trasferTestWrapper(void (*const)());
#pragma endregion

#pragma region Tests declaraction
void testCanCreateTransfer();
void testCannotCreateTransferFromInvalidAccount();
void testCannotCreateTransferToInvalidAccount();
void testCannotCreateTransferForTheSameAccount();
void testCannotCreateTransferInTheFuture();
void testCannotCreateTransferTooFarInThePast();

void testCanUpdateTransfer();
void testCannotUpdateTransferToBecomeFromInvalidAccount();
void testCannotUpdateTransferToBecomeToInvalidAccount();
void testCannotUpdateTransferToBecomeForTheSameAccount();
void testCannotUpdateTransferToBecomeInTheFuture();
void testCannotUpdateTransferToBecomeTooFarInThePast();

void testCanRemoveTransfer();
void testCannotRemoveTransferWithInvalidId();
#pragma endregion

inline void runTransferTestSuite()
{
    // Transfer creation tests
    trasferTestWrapper(testCanCreateTransfer);
    trasferTestWrapper(testCannotCreateTransferFromInvalidAccount);
    trasferTestWrapper(testCannotCreateTransferToInvalidAccount);
    trasferTestWrapper(testCannotCreateTransferForTheSameAccount);
    trasferTestWrapper(testCannotCreateTransferInTheFuture);
    trasferTestWrapper(testCannotCreateTransferTooFarInThePast);

    // Transfer update tests
    trasferTestWrapper(testCanUpdateTransfer);
    trasferTestWrapper(testCannotUpdateTransferToBecomeFromInvalidAccount);
    trasferTestWrapper(testCannotUpdateTransferToBecomeToInvalidAccount);
    trasferTestWrapper(testCannotUpdateTransferToBecomeForTheSameAccount);
    trasferTestWrapper(testCannotUpdateTransferToBecomeInTheFuture);
    trasferTestWrapper(testCannotUpdateTransferToBecomeTooFarInThePast);

    // Transfer deletion tests
    trasferTestWrapper(testCanRemoveTransfer);
    trasferTestWrapper(testCannotRemoveTransferWithInvalidId);
}

#endif // !TRANSFER_TEST_H
