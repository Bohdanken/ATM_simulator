#pragma once
#ifndef CARD_TEST_H
#define CARD_TEST_H

#pragma region Tests declaration
void testCanCreateCard();
void testCannotCreateCardForInvalidAccount();
void testCannotCreateDuplicateCardForAccount();
void testCannotCreateCardWithInvalidPin();

void testCanUpdateCard();
void testCannotUpdateCardBecomingForInvalidAccount();
void testCannotUpdateCardBecomingDuplicateForAccount();
void testCannotUpdateCardWithInvalidPin();

void testCanRemoveCard();
void testCannotRemoveCardWithInvalidId();
#pragma endregion

// TODO: transfer to ATM:
// void testCardReaderValid();
// void testCardReaderInvalid();
// void testAccountLockAfterFailures();

#endif // !CARD_TEST_H