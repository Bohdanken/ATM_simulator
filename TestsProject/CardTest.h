#pragma once
#ifndef CARD_TEST_H
#define CARD_TEST_H

#pragma region Utils
void cardTestWrapper(void (*const)());
#pragma endregion

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

inline void runCardTestSuite()
{
    // Card creation tests
    cardTestWrapper(testCanCreateCard);
    cardTestWrapper(testCannotCreateCardForInvalidAccount);
    cardTestWrapper(testCannotCreateDuplicateCardForAccount);
    cardTestWrapper(testCannotCreateCardWithInvalidPin);

    // Card update tests  
    cardTestWrapper(testCanUpdateCard);
    cardTestWrapper(testCannotUpdateCardBecomingForInvalidAccount);
    cardTestWrapper(testCannotUpdateCardBecomingDuplicateForAccount);
    cardTestWrapper(testCannotUpdateCardWithInvalidPin);

    // Card deletion tests
    cardTestWrapper(testCanRemoveCard);
    cardTestWrapper(testCannotRemoveCardWithInvalidId);
}

#endif // !CARD_TEST_H