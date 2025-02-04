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

#include <random>
#include <cstddef>
#include <cstdint>
#include "block.hpp"

using std::random_device;
using std::mt19937;
using std::uniform_int_distribution;
using std::size_t;
using std::uint8_t;

fc::Block::Block() {
    // Initialize block with default values.
    bytes = {
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    };

    // Configure offset.
    offset = 0;

    // Now it is empty block.
    realSize = 0;
}

fc::Block fc::Block::Generate() {
    // Create a new empty block.
    Block newBlock;

    // Create a new random device.
    random_device randomDevice;

    // Use 32-bit 'Mersenne Twister' algorithm.
    mt19937 generator(randomDevice());

    // Configure generator to produce numbers from 0 to UINT8_MAX.
    uniform_int_distribution<unsigned int> distribute(0, UINT8_MAX);

    // Generate block by bytes.
    for (size_t counter = 0; counter < SIZE; counter++) {
        // Produce pseudo-random number and store it.
        Push(static_cast<uint8_t>(distribute(generator)));
    }

    // Return the block.
    return newBlock;
}

void fc::Block::Push(const uint8_t byte) {
    // Check if block isn't full.
    if (realSize != SIZE) {
        // Append one byte to the end of block.
        bytes[offset] = byte;

        // Update block real size.
        realSize++;

        // Update offset if it is possible.
        if (realSize < SIZE) {
            offset++;
        }
    }
}

void fc::Block::Encrypt(const fc::Block& key) noexcept {
    // Encrypt block within 15 rounds.
    for (int round = 0; round < 15; round++) {
        // Step 1: swap bytes.
        for (size_t index = 1, counter = 0, pairs = realSize / 2; counter < pairs; index += 2, counter++) {
            // Copy 'index - 1 byte' to the temporary storage.
            const auto temp = bytes[index - 1];

            // Move 'index byte'.
            bytes[index - 1] = bytes[index];

            // Store 'index - 1 byte'.
            bytes[index] = temp;
        }

        // Step 2: get round key.
        const auto roundKey = key.GetRoundVersion(round);

        // Step 3: xor key.
        for (size_t index = 0; index < realSize; index++) {
            bytes[index] ^= roundKey[index];
        }
    }
}

void fc::Block::Decrypt(const fc::Block& key) noexcept {
    // Decrypt block within 15 rounds.
    for (int round = 14; round >= 0; round--) {
        // Step 1: get round key.
        const auto roundKey = key.GetRoundVersion(round);

        // Step 2: xor key.
        for (std::size_t index = 0, size = bytes.size(); index < size; index++) {
            bytes[index] ^= roundKey[index];
        }

        // Step 3: swap bytes.
        for (size_t index = 1, counter = 0, pairs = realSize / 2; counter < pairs; index += 2, counter++) {
            // Copy 'index - 1 byte' to the temporary storage.
            const auto temp = bytes[index - 1];

            // Move 'index byte'.
            bytes[index - 1] = bytes[index];

            // Store 'index - 1 byte'.
            bytes[index] = temp;
        }
    }
}

fc::Block fc::Key::GetRoundVersion(const int round) const {
    // Create new empty round block.
    Block newRoundBlock;

    // Calculate a 'magic' number.
    const auto magicNumber = static_cast<uint8_t>(0x4D ^ round);

    // Combine block and the 'magic' number.
    for (size_t index = 0; index < realSize; index++) {
        newRoundBlock.bytes[index] = bytes[index] ^ magicNumber;
    }

    // Return round block.
    return newRoundBlock;
}
