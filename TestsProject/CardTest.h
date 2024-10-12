#pragma once
#ifndef CARD_TEST_H
#define CARD_TEST_H

void testUpdatePin();
void testVerifyPinCorrect();
void testVerifyPinIncorrect();
void testAccountLockAfterFailures();

// TODO: transfer to ATM:
// void testCardReaderValid();
// void testCardReaderInvalid();

#endif // !CARD_TEST_H