/*
** Copyright (C) 2025 Vitaliy Tarasenko.
**
** This file is part of FishCode (fishcode).
**
** FishCode is free software: you can redistribute it and/or modify it under
** the terms of the GNU General Public License as published by the Free
** Software Foundation, either version 3 of the License, or (at your option)
** any later version.
**
** FishCode is distributed in the hope that it will be useful, but WITHOUT
** ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
** FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
** more details.
**
** You should have received a copy of the GNU General Public License along
** with FishCode. If not, see <https://www.gnu.org/licenses/>.
*/

#include <array>
#include <string>
#include <cstddef>
#include <cstdint>
#include "password.hpp"

fc::Password::Password(const std::string& passwordString) {
    // Get length of the string (in bytes).
    const auto length = passwordString.length();

    // Create storage for the new password bytes.
    std::array<std::uint8_t, SIZE> newBytes;

    // Convert each symbol into its binary representation and store it.
    for (std::size_t index = 0; index < length; index++) {
        newBytes[index] = static_cast<std::uint8_t>(passwordString[index]);
    }

    // Check if password string doesn't have maximal length.
    if (length < SIZE) {
        // Append additional bytes from the beginning.
        for (std::size_t counter = length, index = 0; counter < SIZE; counter++, index++) {
            newBytes[counter] = newBytes[index];
        }
    }

    // Store new password bytes.
    SetBytes(newBytes);

    // Set up password size.
    SetRealSize(SIZE);
}
