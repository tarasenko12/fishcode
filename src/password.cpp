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

#include <string>
#include <cstddef>
#include <cstdint>
#include "password.hpp"

using std::string;
using std::size_t;
using std::uint8_t;

fc::Password::Password(const string& passwordString) {
    // Get length of the string (in bytes).
    const auto length = passwordString.length();

    // Convert each symbol into its binary representation and store it.
    for (const auto symbol : passwordString) {
        Push(static_cast<uint8_t>(symbol));
    }

    // Check if password string doesn't have maximal length.
    if (length < SIZE) {
        // Append additional bytes from the beginning.
        for (size_t counter = length, index = 0; counter < SIZE; counter++, index++) {
            Push(*(this)[index]);
        }
    }
}
