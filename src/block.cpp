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
#include <cstddef>
#include <cstdint>
#include "block.hpp"
#include "key.hpp"

fc::Block::Block(std::array<std::uint8_t, fc::Block::SIZE>&& newBytes, const std::size_t newRealSize) {
    // Initialize block bytes.
    bytes = newBytes;

    // Store real size of the block.
    realSize = newRealSize;
}

void fc::Block::Encrypt(const fc::Key& key) {
    // Encrypt block within 15 rounds.
    for (int round = 0; round < 15; round++) {
        // Step 1: swap bytes.
        for (std::size_t index = 1, counter = 0, pairs = realSize / 2; counter < pairs; index += 2, counter++) {
            // Copy 'index - 1 byte' to the temporary storage.
            const auto temp = bytes[index - 1];

            // Move 'index byte'.
            bytes[index - 1] = bytes[index];

            // Store 'index - 1 byte'.
            bytes[index] = temp;
        }

        // Step 2: get round key.
        const auto roundKey = key.GetRoundKey(round);

        // Step 3: xor key.
        for (std::size_t index = 0; index < realSize; index++) {
            bytes[index] ^= roundKey.bytes[index];
        }
    }
}

void fc::Block::Decrypt(const fc::Key& key) {
    // Decrypt block within 15 rounds.
    for (int round = 14; round >= 0; round--) {
        // Step 1: get round key.
        const auto roundKey = key.GetRoundKey(round);

        // Step 2: xor key.
        for (std::size_t index = 0, size = bytes.size(); index < size; index++) {
            bytes[index] ^= roundKey.bytes[index];
        }

        // Step 3: swap bytes.
        for (std::size_t index = 1, counter = 0, pairs = realSize / 2; counter < pairs; index += 2, counter++) {
            // Copy 'index - 1 byte' to the temporary storage.
            const auto temp = bytes[index - 1];

            // Move 'index byte'.
            bytes[index - 1] = bytes[index];

            // Store 'index - 1 byte'.
            bytes[index] = temp;
        }
    }
}
