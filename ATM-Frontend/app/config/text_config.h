#pragma once
#ifndef TEXT_CONFIG_H
#define TEXT_CONFIG_H

namespace ATM::Config
{
#pragma region General
    /* Default element-to-element Y-axis padding */
    constexpr float Y_PADDING = 3.0f;
#pragma endregion

#pragma region Account number
    /* Account number */
    constexpr const char *ACCOUNT_NUMBER_LABEL = "Enter your account number (12 digits):";

    /* Currently being discarded due to "##" */
    constexpr const char *ACCOUNT_NUMBER_INPUT_TITLE = "##account number";

    /*
     * A template of an account number with trailing padding
     * 12 digits for the number itself
     * 4 '_' characters for padding
     */
    constexpr const char *ACCOUNT_NUMBER_TEMPLATE = "000011112222___";
#pragma endregion

#pragma region Account pin code
    /* Account pin code */
    constexpr const char *PIN_LABEL = "Enter your pin code (4 digits):";

    /* Currently being discarded due to "##" */
    constexpr const char *PIN_INPUT_TITLE = "##pin";

    /*
     * A template of a pin code with trailing padding
     * 4 digits for the pin code itself
     * 4 '_' characters for padding
     */
    constexpr const char *PIN_TEMPLATE = "0000____";
#pragma endregion
}

#endif // !TEXT_CONFIG_H
